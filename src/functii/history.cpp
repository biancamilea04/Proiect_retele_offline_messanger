#include "functii/history.h"

using namespace std;

void fn_history(vector<string> args, string &msgrasp, int id)
{

    if (id == 0)
    {
        msgrasp = "[history]Comanda invalida. Va rog sa va logati!";
        return;
    }

    if (args.size() != 2)
    {
        msgrasp = "[history]Comanda invalida. Incearca \"history [USERNAME]\".";
        return;
    }

    string user = args[1];
    int id_user = get_id_by_user(user, msgrasp);
    if (id_user == 0)
    {
        msgrasp = "User invalid";
        return;
    }

    if(id_user==id){
        msgrasp = "Nu poti vedea istoricul cu tine";
        return;
    }

    sqlite3 *bd;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "Eroare la deschiderea bazei de date.\n";
        return;
    }

    const char *sql = R"(
        SELECT 
            sender.username AS sender_name,
            mesaje.mesaj AS mesaj_sent,
            receiver.username AS receiver_name
        FROM mesaje
        JOIN users AS sender ON mesaje.id_sender = sender.id
        JOIN users AS receiver ON mesaje.id_receive = receiver.id
        WHERE (mesaje.id_sender = ? AND mesaje.id_receive = ?) OR (mesaje.id_sender = ? AND mesaje.id_receive = ?)
        ORDER BY mesaje.id;
    )";

    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea interogării SQL.\n";
        printf("[send]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, id_user);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, id);
    sqlite3_bind_int(stmt, 4, id_user);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        while (rc == SQLITE_ROW)
        {
            std::string sender_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            std::string mesaj_sent = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            std::string receiver_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

            msgrasp += '\n';
            msgrasp += sender_name;
            msgrasp += '>';
            msgrasp += mesaj_sent;

            rc = sqlite3_step(stmt);
        }
    }
    else
    {
        msgrasp = "Conversatie goala";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}