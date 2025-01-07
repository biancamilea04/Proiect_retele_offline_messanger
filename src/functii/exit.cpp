#include "functii/exit.h"

using namespace std;

void fn_exit(vector<string> args, string &msgrasp, int &id)
{
    if (args.size() != 1)
    {
        msgrasp = "[exit]Comanda incorecta. Incearca \"exit\"";
        return;
    }
    else
    {

        if (id != 0)
        {
            sqlite3_stmt *Stmt1;
            sqlite3 *bd;

            int rc = sqlite3_open("bd/users.db", &bd);
            if (rc)
            {
                msgrasp = "Eroare la deschiderea bazei de date.";
                perror("[exit]Eroare la deschiderea bazei de date: ");
                return;
            }

            string comanda1 = "UPDATE users SET status = 'offline' WHERE id = ?;";

            rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &Stmt1, nullptr);
            if (rc != SQLITE_OK)
            {
                msgrasp = "Eroare la pregatirea comenzii SQL pentru actualizarea statusului utilizatorului offline";
                printf("[exit]Eroare la pregatirea comenzii de actualizare: %s\n", sqlite3_errmsg(bd));
                sqlite3_finalize(Stmt1);
                sqlite3_close(bd);
                return;
            }

            sqlite3_bind_int(Stmt1, 1, id);

            rc = sqlite3_step(Stmt1);
            if (rc == SQLITE_DONE)
            {
                msgrasp = "Exit!";
                printf("[exit]logout reușit \n");
            }
            else
            {
                msgrasp = "Eroare la actualizarea statusului utilizatorului.";
                printf("[exit]Eroare la actualizarea statusului: %s\n", sqlite3_errmsg(bd));
            }

            sqlite3_finalize(Stmt1);
            sqlite3_close(bd);
            id = 0;
        }
        else
        {
            msgrasp = "Exit!";
        }
    }
}
