#include "functii/register.h"

using namespace std;

void fn_register(vector<string> args, string &msgrasp)
{
    if (args.size() != 4)
    {
        msgrasp = "[register]Comanda invalida. Incearca \"register <username> <parola> <parola_confirmata>\"";
        printf("[register]Numar de argumente invalid\n");
        return;
    }

    if (args[2] != args[3])
    {
        msgrasp = "Parolele nu se potrivesc";
        printf("[register]Parolele nu se potrivesc\n");
        return;
    }

    sqlite3 *bd;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "Eroare la deschiderea bazei de date.";
        printf("[register]Eroare la deschiderea bazei de date: %s\n", sqlite3_errmsg(bd));
        return;
    }

    sqlite3_stmt *stmt;
    string user = args[1];
    string pswd = args[2];

    string comanda1 = "SELECT id FROM users WHERE username = ? AND password = ?;";
    rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
        printf("[register]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pswd.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        msgrasp = "Utilizator existent";
        printf("[register]Username deja exista: %s\n", user.c_str());
        sqlite3_finalize(stmt);
        sqlite3_close(bd);
        return;
    }
    sqlite3_finalize(stmt);

    string comanda2 = "SELECT id FROM users WHERE username = ?;";
    rc = sqlite3_prepare_v2(bd, comanda2.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
        printf("[register]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        msgrasp = "Username utilizat. Incearca altul.";
        printf("[register]Username deja folosit: %s\n", user.c_str());
        sqlite3_finalize(stmt);
        sqlite3_close(bd);
        return;
    }
    sqlite3_finalize(stmt);

    string comanda3 = "INSERT INTO users (username, password) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(bd, comanda3.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregatirea comenzii SQL pentru inregistrare";
        printf("[register]Eroare la pregatirea comenzii de inregistrare: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pswd.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
    {
        msgrasp = "Inregistrare reusita";
        printf("[register]Utilizatorul a fost inregistrat cu succes\n");
    }
    else
    {
        msgrasp = "Eroare la inregistrare";
        printf("[register]Eroare la inregistrare: %s\n", sqlite3_errmsg(bd));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}