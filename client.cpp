#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "biblioteci.h"
#include <comunic/comunicare.h>
#include <functii/notificariMesaje.h>
#include <functii/baze_de_date.h>
#include <const.h>

// ./client 127.0.0.1 2024

extern int errno;

int port;

struct User
{
    int id;
    string username;
};

int main(int argc, char *argv[])
{
    int sd;
    struct sockaddr_in server;
    char msg[2024];
    char response[2024];
    User user = {0, "client"};

    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    printf("[client]Welcome!.\n");

    while (1)
    {
        printf("[%s]> ", user.username.c_str());
        bzero(msg, 2024);
        fgets(msg, 100, stdin);
        vector<string> args = prelucram_comanda(msg);
        msg[strcspn(msg, "\n")] = 0;

        if (strcmp(msg, S_EXIT) == 0)
        {
            printf("[client]Deconectare.\n");
            if (fn_write(sd, msg, strlen(msg) + 1) <= 0)
            {
                printf("[client]Eroare la scrierea in socket\n");
                break;
            }
            break;
        }
        else

            if (fn_write(sd, msg, strlen(msg) + 1) <= 0)
        {
            printf("[client]Eroare la scrierea in socket\n");
            break;
        }

        int size;
        bzero(response, 2024);

        if (fn_read(sd, response, &size) <= 0)
        {
            printf("[client]Eroare la read() de la server.\n");
            break;
        }

        if (strncmp(response, "Succes login!", strlen("Succes login!")) == 0)
        {
            string mesaj_functie;
            user.username = args[1];
            user.id = get_id_by_user(user.username, mesaj_functie);
        }
        else if (strcmp(response, "Succes logout!") == 0)
        {
            string mesaj_functie;
            user.username = "client";
            user.id = 0;
        }

        printf("[%s]  %s\n", user.username.c_str(), response);
    }

    close(sd);
    return 0;
}