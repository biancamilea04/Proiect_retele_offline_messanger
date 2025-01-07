#include "functii/logout.h"

using namespace std;

void fn_logout(vector<string> args, string &msgrasp, int &id)
{
  if (args.size() != 1)
  {
    msgrasp="[logout]Comanda invalida. Incearca \"logout\"";
    return;
  }

  if (id == 0)
  {
    msgrasp = "Niciun utilizator logat.";
    return;
  }
  else
  {
    sqlite3_stmt *Stmt1;
    sqlite3 *bd;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
      msgrasp = "Eroare la deschiderea bazei de date.";
      perror("[logout]Eroare la deschiderea bazei de date: ");
      return;
    }

    string comanda1 = "UPDATE users SET status = 'offline' WHERE id = ?;";
    
    rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &Stmt1, nullptr);
    if (rc != SQLITE_OK)
    {
      msgrasp = "Eroare la pregatirea comenzii SQL pentru actualizarea statusului utilizatorului offline";
      printf("[logout]Eroare la pregatirea comenzii de actualizare: %s\n", sqlite3_errmsg(bd));
      sqlite3_finalize(Stmt1);
      sqlite3_close(bd);
      return;
    }

    sqlite3_bind_int(Stmt1, 1, id);

    rc = sqlite3_step(Stmt1);
    if (rc == SQLITE_DONE)
    {
      msgrasp = "Succes logout!";
      printf("[logout]logout reușit \n");
    }
    else
    {
      msgrasp = "Eroare la actualizarea statusului utilizatorului.";
      printf("[logout]Eroare la actualizarea statusului: %s\n", sqlite3_errmsg(bd));
    }

    sqlite3_finalize(Stmt1);
    sqlite3_close(bd);
    id=0;
  }


}