#include "functii/offlinemessage.h"

using namespace std;

void fn_mesajeNecitite(vector<string> args,string &msgrasp, int id)
{
    sqlite3 *bd;
    sqlite3_stmt *stmt;

    if (id == 0)
    {
        msgrasp = "Utilizatorul nu este autentificat.";
        return;
    }

    if (args.size() != 1)
    {
        msgrasp = "[mesajeNecitite]Comanda invalida. Incearca \"mesajeNecitite\"";
        return;
    }

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la deschiderea bazei de date.";
        perror("[mesajeNecitite]Eroare la deschiderea bazei de date: ");
        return;
    }

    const char *sql = R"(
        SELECT DISTINCT u.username
        FROM mesaje m
        JOIN users u ON m.id_sender = u.id
        WHERE m.id_receive = ? AND m.status_mesaj = 'necitit' AND m.status_receiver='offline';
    )";

    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL.";
        printf("[mesajeNecitite]Eroare la pregătirea comenzii: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    vector<string> userList;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *username = sqlite3_column_text(stmt, 0);
        userList.push_back(reinterpret_cast<const char *>(username));
    }

    if (userList.empty())
    {
        msgrasp += "\nNu ai mesaje necitite.";
    }
    else
    {
        msgrasp += "\nAi mesaje necitite de la:\n";
        for (size_t i = 0; i < userList.size(); ++i)
        {
            msgrasp += userList[i];
            msgrasp += "\n";
        }
       // msgrasp+="Foloseste comanda ShowUnreadMessage [USERNAME] pentru a vedea mesajele";

    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}

void fn_showunreadmessage(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "[unreadMessage]Comanda invalida. Utilizator nelogat.";
        return;
    }

    if (args.size() != 2)
    {
        msgrasp = "[unreadMessage]Comanda invalida. Incearca \"ShowUnreadMessage [USERNAME]\".";
        return;
    }

    string username = args[1];
    sqlite3 *bd;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la deschiderea bazei de date.";
        perror("[unreadMessage]Eroare la deschiderea bazei de date: ");
        return;
    }

    const char *sql_find_sender = R"(
        SELECT id FROM users WHERE username = ?;
    )";

    rc = sqlite3_prepare_v2(bd, sql_find_sender, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL pentru găsirea utilizatorului.";
        printf("[unreadMessage](1)Eroare la pregătirea comenzii: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int sender_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        sender_id = sqlite3_column_int(stmt, 0);
    }
    else
    {
        msgrasp = "[unreadMessage]Nu exista acest username";
        sqlite3_finalize(stmt);
        sqlite3_close(bd);
        return;
    }

    sqlite3_finalize(stmt);

    const char *sql_get_unread = R"(
        SELECT m.mesaj 
        FROM mesaje m
        WHERE m.id_sender = ? AND m.id_receive = ? AND m.status_mesaj = 'necitit';
    )";

    rc = sqlite3_prepare_v2(bd, sql_get_unread, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL pentru selectarea mesajelor.";
        printf("[unreadMessage](2)Eroare la pregătirea comenzii: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, sender_id);
    sqlite3_bind_int(stmt, 2, id);

    vector<string> messages;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *message = sqlite3_column_text(stmt, 0);
        messages.push_back(username + ">" + reinterpret_cast<const char *>(message));
    }

    sqlite3_finalize(stmt);

    if (messages.empty())
    {
        msgrasp = "[unreadMessage]Nu ai mesaje necitite de la acest utilizator.";
        sqlite3_close(bd);
        return;
    }

    const char *sql_update_status = R"(
        UPDATE mesaje
        SET status_mesaj = 'citit'
        WHERE id_sender = ? AND id_receive = ? AND status_mesaj = 'necitit';
    )";

    rc = sqlite3_prepare_v2(bd, sql_update_status, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL pentru actualizarea mesajelor.";
        printf("[unreadMessage](3)Eroare la pregătirea comenzii: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, sender_id);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        msgrasp = "Eroare la actualizarea statusului mesajelor.";
        printf("[unreadMessage]Eroare la actualizarea statusului: %s\n", sqlite3_errmsg(bd));
        sqlite3_finalize(stmt);
        sqlite3_close(bd);
        return;
    }

    sqlite3_finalize(stmt);

    msgrasp = "Mesaje necitite:\n";
    for (const auto &msg : messages)
    {
        msgrasp += msg + "\n";
    }
    msgrasp.pop_back();

    sqlite3_close(bd);
}
