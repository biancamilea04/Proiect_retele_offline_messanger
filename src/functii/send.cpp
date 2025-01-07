#include "functii/send.h"

using namespace std;

void fn_send(vector<string> args, string &msgrasp, int id)
{
   if (id == 0)
   {
      msgrasp = "[send]Comanda invalida. Va rog sa va logati!";
      return;
   }

   if (args.size() < 3 || args[1][args[1].size() - 1] != ':')
   {
      msgrasp = "[send]Comanda invalida. Incearca \"send [USERNAME]: [MESAJ]\"";
      return;
   }

   string user = args[1];
   user.pop_back();
   string mesaj;

   for (long unsigned int i = 2; i < args.size(); i++)
   {
      for (auto c : args[i])
      {
         mesaj.push_back(c);
      }
      mesaj.push_back(' ');
   }

   while (mesaj[mesaj.size()-1]==' ')
   {
      mesaj.pop_back();
   }

   while(mesaj[0]==' '){
      for(long unsigned int j = 0 ; j < mesaj.size()-1; j++){
         mesaj[j]=mesaj[j+1];
      }
      mesaj.pop_back();
   }

   int id_receiver = get_id_by_user(user, msgrasp);
   if (id_receiver == -1)
   {
      msgrasp = "[send]Eroare get id users";
      return;
   }
   else if (id_receiver == 0)
   {
      msgrasp = "Userul nu s-a gasit.";
      return;
   }
   else if (id_receiver == id)
   {
      msgrasp = "Nu-ti poti trimite mesaje singur.";
      return;
   }
   else
   {
      msgrasp = mesaj;
      string status_receiver;
      int st = get_status_by_id(id_receiver, msgrasp);
      if (st == -1)
      {
         return;
      }
      else if (st == 0)
      {
         status_receiver = "offline";
      }
      else if (st == 1)
      {
         status_receiver = "online";
      }

      sqlite3 *bd;
      mesaj=trim(mesaj);

      int rc = sqlite3_open("bd/users.db", &bd);
      if (rc)
      {
         msgrasp = "[send]Eroare la deschiderea bazei de date.";
         return;
      }

      const char *sql = R"(
            INSERT INTO mesaje (id_sender, id_receive, mesaj, status_mesaj, status_receiver,data_mesaj)
            VALUES (?, ?, ?, 'necitit', ?,datetime('now'));
        )";

      sqlite3_stmt *stmt;
      rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, 0);
      if (rc != SQLITE_OK)
      {
         msgrasp = "[send]Eroare la pregătirea interogării SQL.";
         printf("[send]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
         sqlite3_finalize(stmt);
         sqlite3_close(bd);
         return;
      }

      sqlite3_bind_int(stmt, 1, id);
      sqlite3_bind_int(stmt, 2, id_receiver);
      sqlite3_bind_text(stmt, 3, mesaj.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 4, status_receiver.c_str(), -1, SQLITE_STATIC);

      printf("[send]mesaj trimis:#%s#\n",mesaj.c_str());

      rc = sqlite3_step(stmt);
      if (rc != SQLITE_DONE)
      {
         msgrasp = "[send]Eroare la inserarea mesajului în baza de date.";
      }
      else
      {
         msgrasp = "Mesaj trimis cu succes.";
      }

      sqlite3_finalize(stmt);
      sqlite3_close(bd);
   }
}
