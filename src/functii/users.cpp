#include "functii/users.h"

void fn_users(vector<string> args, string &msgrasp, int id)
{

    if (id == 0)
    {
        msgrasp = "[users]Comanda invalida. Va rog sa va logati!";
        return;
    }

    if (args.size() != 1)
    {
        msgrasp = "[users]Comanda invalida. Incearca comanda \"users\".";
        return;
    }

    sqlite3_stmt *stmt;
    sqlite3 *bd;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "Eroare la deschiderea bazei de date.";
        perror("[users]Eroare la deschiderea bazei de date: ");
        return;
    }

    string comanda1 = "SELECT username,status FROM users";
    rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
        printf("[register]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char *status =reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        msgrasp += "\n"; 
        msgrasp += username;
        msgrasp += " ";
        msgrasp += status;  
        
    }
    
    if(msgrasp.length()==0){
        msgrasp="Nu a fost inregistrat niciun utilizator";
    } else 

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}