
#include "functii/login.h"

using namespace std;

void fn_login(vector<string> args, string &msgrasp, int &id)
{
  if (id != 0)
  {
    msgrasp = "[login]Utilizator deja logat!";
    return;
  }

  if (args.size() != 3)
  {
    msgrasp = "[login]Comanda invalida. Incearca \"login [usernam] [PAROLA]\"";
    return;
  }

  string user = args[1];
  string pswd = args[2];

  sqlite3_stmt *stmt;
  sqlite3 *bd;

  int rc = sqlite3_open("bd/users.db", &bd);
  if (rc)
  {
    msgrasp = "Eroare la deschiderea bazei de date.";
    perror("[login]Eroare la deschiderea bazei de date: ");
    return;
  }

  string comanda1 = "SELECT id FROM users WHERE username = ? AND password = ?;";
  rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK)
  {
    msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
    printf("[login]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
    sqlite3_close(bd);
    return;
  }

  sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, pswd.c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW)
  {
    id = sqlite3_column_int(stmt, 0);
    string comanda2 = "UPDATE users SET status = 'online' WHERE username = ? AND password = ?;";
    sqlite3_stmt *Stmt1;

    rc = sqlite3_prepare_v2(bd, comanda2.c_str(), -1, &Stmt1, nullptr);
    if (rc != SQLITE_OK)
    {
      msgrasp = "Eroare la pregatirea comenzii SQL pentru actualizarea statusului utilizatorului";
      printf("[login]Eroare la pregatirea comenzii de actualizare: %s\n", sqlite3_errmsg(bd));
      sqlite3_finalize(Stmt1);
      sqlite3_close(bd);
      return;
    }

    sqlite3_bind_text(Stmt1, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(Stmt1, 2, pswd.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(Stmt1);
    if (rc == SQLITE_DONE)
    {

      msgrasp = "Succes login! ";
      msgrasp += to_string(id);     
      printf("[login]Login reușit pentru utilizatorul %s. Status actualizat la 'online'.\n", user.c_str());
      
      printf("[Login]Mesaje in astepare: %s",msgrasp.c_str());
    }
    else
    {
      msgrasp = "Eroare la actualizarea statusului utilizatorului.";
      printf("[login]Eroare la actualizarea statusului: %s\n", sqlite3_errmsg(bd));
    }

    sqlite3_finalize(Stmt1);
  }
  else
  {
    string comanda3 = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt *Stmt3;

    rc = sqlite3_prepare_v2(bd, comanda3.c_str(), -1, &Stmt3, nullptr);
    if (rc != SQLITE_OK)
    {
      msgrasp = "Eroare la pregatirea comenzii SQL pentru actualizarea statusului utilizatorului";
      printf("[login]Eroare la pregatirea comenzii de actualizare: %s\n", sqlite3_errmsg(bd));
      sqlite3_finalize(Stmt3);
      sqlite3_close(bd);
      return;
    }

    sqlite3_bind_text(Stmt3, 1, user.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(Stmt3);
    if (rc == SQLITE_DONE)
    {
      msgrasp = "Nu exista acest username!";
      printf("[login]Username invalid: %s.\n", user.c_str());
      sqlite3_finalize(Stmt3);
      sqlite3_close(bd);
      return;
    }
    else
    {
      msgrasp = "Parola gresita!";
      printf("[login]Parola gresita pentru user: %s.\n", user.c_str());
      sqlite3_finalize(Stmt3);
      sqlite3_close(bd);
      return;
    }
  }

  sqlite3_finalize(stmt);
  sqlite3_close(bd);
}