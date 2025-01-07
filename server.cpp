
#include "biblioteci.h"

#include "const.h"
#include "functii/exit.h"
#include "functii/login.h"
#include "functii/logout.h"
#include "functii/help.h"
#include "functii/history.h"
#include "functii/reply.h"
#include "functii/send.h"
#include "functii/users.h"
#include "functii/register.h"
#include "functii/baze_de_date.h"
#include "functii/offlinemessage.h"
#include "functii/notificariMesaje.h"
#include "functii/grup.h"

#include "comunic/comunicare.h"

using namespace std;

#define PORT 2025

extern int errno;

void *prelucram(void *client_sock)
{
    string msgrasp = "";
    char msg[1024];
    int client = *(int *)client_sock;
    int id = 0;

    BazaDeDateUsers();
    BazaDeDateMesaje();
    BazaDeDateGrupuri();

    do
    {
        int size;
        if (fn_read(client, msg, &size) <= 0)
        {
            printf("[server]Eroare la read() de la client.\n");
            close(client);
            return nullptr;
        }

        vector<string> args = prelucram_comanda(msg);

        string comanda = trim(args[0]);

        if (comanda == S_EXIT)
        {
            fn_exit(args, msgrasp, id);
            printf("[server]Mesajul a fost transmis: %s\n", msgrasp.c_str());
            break;
        }
        else if (comanda == LOGIN)
        {
            fn_login(args, msgrasp, id);
        }
        else if (comanda == REGISTER)
        {
            fn_register(args, msgrasp);
        }
        else if (comanda == LOGOUT)
        {
            fn_logout(args, msgrasp, id);
        }
        else if (comanda == SEND)
        {
            fn_send(args, msgrasp, id);
        }
        else if (comanda == SENDGROUP)
        {
            fn_sendGroup(args, msgrasp, id);
        }
        else if (comanda == REPLY)
        {
            fn_reply(args, msgrasp, id);
        }
        else if (comanda == HELP)
        {
            fn_help(args, msgrasp);
        }
        else if (comanda == HISTORY)
        {
            fn_history(args, msgrasp, id);
        }
        else if (comanda == HISTORYGROUP)
        {
            fn_historyGroup(args, msgrasp, id);
        }
        else if (comanda == USERS)
        {
            fn_users(args, msgrasp, id);
        }
        else if (comanda == OFFLINE)
        {
            fn_showunreadmessage(args, msgrasp, id);
        }
        else if (comanda == CREATE)
        {
            fn_createGrup(args, msgrasp, id);
        }
        else if (comanda == ADD)
        {
            fn_addMembers(args, msgrasp, id);
        }
        else if (comanda == GROUP)
        {
            fn_group(args, msgrasp, id);
        }
        else if (comanda == GROUPMEMBERS)
        {
            fn_groupMembers(args, msgrasp, id);
        }
        else if (comanda == REPLYGROUP)
        {
            fn_replyGroup(args, msgrasp, id);
        }
        else if (comanda == NOTIFICARE)
        {
            fn_notificareMesajNou(args, msgrasp, id);
        }
        else if (comanda == NECITITA)
        {
            fn_mesajeNecitite(args, msgrasp, id);
        }
        else
        {
            msgrasp = "comanda nerecunoscuta. Incearca comanda help";
        }

        printf("[server] rezultat prelucrare:\n %s\n", msgrasp.c_str());

        if (msgrasp.size() == 0)
        {
            msgrasp = "Niciun raspuns de la server";
        }

        if (fn_write(client, const_cast<char *>(msgrasp.c_str()), msgrasp.length()) <= 0)
        {
            printf("[server]Mesajul: %s\n", msgrasp.c_str());
            printf("[server]Eroare la fn_write() catre client.\n");
            return nullptr;
        }
        else
        {
            printf("[server]Mesajul a fost transmis\n");
        }
        msgrasp = "";
    } while (1);

    close(client);
    return nullptr;
}

int main()
{
    printf("[server]\n");
    struct sockaddr_in server;
    struct sockaddr_in from;
    string msgrasp = "";
    int sd;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, 5) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    while (1)
    {
        int client;
        unsigned int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        client = accept(sd, (struct sockaddr *)&from, &length);
        if (client < 0)
        {
            printf("Eroare la accept\n");
            perror("Eroarea ");
            return errno;
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, prelucram, (void *)&client) != 0)
        {
            perror("[server]Eroare la creare thread");
            close(client);
            continue;
        }

        pthread_detach(thread);
    }
    printf("[server] close sd\n");
    close(sd);
}