#include "functii/notificariMesaje.h"

using namespace std;

void fn_notificareMesajNou(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "Utilizatorul nu este autentificat.";
        return;
    }

    if (args.size() != 1)
    {
        msgrasp = "[MesajNou]Comanda invalida. Incearca \"notificare\"";
        return;
    }

    sqlite3 *bd;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "Eroare la deschiderea bazei de date.";
        perror("[notificareMesajNou] Eroare la deschiderea bazei de date: ");
        return;
    }

    string sql = R"(
        SELECT DISTINCT u.username, m.id
        FROM mesaje m
        INNER JOIN users u ON m.id_sender = u.id
        WHERE m.id_receive = ? AND m.status_mesaj = 'necitit' AND m.notificare_trimisa = 0 AND m.status_receiver='online';
    )";

    rc = sqlite3_prepare_v2(bd, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL pentru notificare mesaje.";
        printf("[notificareMesajNou] Eroare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *sender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        int message_id = sqlite3_column_int(stmt, 1);

        msgrasp += "\n[" + string(sender) + "] ți-a trimis un mesaj";

        string updateSql = "UPDATE mesaje SET notificare_trimisa = 1 WHERE id = ?;";
        sqlite3_stmt *updateStmt;

        rc = sqlite3_prepare_v2(bd, updateSql.c_str(), -1, &updateStmt, nullptr);
        if (rc == SQLITE_OK)
        {
            sqlite3_bind_int(updateStmt, 1, message_id);
            sqlite3_step(updateStmt);
            sqlite3_finalize(updateStmt);
        }
        else
        {
            printf("[notificareMesajNou] Eroare la actualizarea notificării: %s\n", sqlite3_errmsg(bd));
        }
    }

    if (msgrasp == "")
    {
        msgrasp="Nu ai notificari";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}
