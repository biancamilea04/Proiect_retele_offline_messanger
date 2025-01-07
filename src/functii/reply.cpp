#include "functii/reply.h"

using namespace std;

void create_mesaje(vector<string> args, string &mesaj_reply, string &mesaj_send, int start ,string &msgrasp)
{
   long unsigned int i = start;
   do
   {
      printf("(1)args[%ld]=%s\n", i, args[i].c_str());
      mesaj_reply += args[i];
      mesaj_reply += ' ';
      i += 1;
   } while (args[i - 1][args[i - 1].size() - 1] != ':' && i < args.size());

   if (i == args.size())
   {
      printf("valoare lui i=%ld", i);
      msgrasp = "[reply] Comanda invalida(6). Incearca reply [USERNAME] [MESAJ]: [MESAJ]";
      return;
   }

   mesaj_reply = trim(mesaj_reply);
   mesaj_reply.pop_back();

   while (i < args.size())
   {
      mesaj_send += args[i];
      mesaj_send += ' ';
      i += 1;
   }
   mesaj_send = trim(mesaj_send);
}

void fn_reply(vector<string> args, string &msgrasp, int id)
{
   if (id == 0)
   {
      msgrasp = "[reply] Comanda invalida(4). Utilizator nelogat";
      printf("[reply] id=%d\n", id);
      return;
   }

   if (args.size() < 4)
   {
      msgrasp = "[reply] Comanda invalida(5). Incearca reply [USERNAME] [MESAJ]: [MESAJ]";
      printf("[reply] nr_argumente=%lu\nargumentul[2]=%s\n", args.size(), args[2].c_str());
      return;
   }

   string user = args[1];
   int id_user = get_id_by_user(user, msgrasp);

   if (id_user == -1)
   {
      return;
   }

   if (id_user == 0)
   {
      msgrasp = "Nu exista acest utilizator";
      return;
   }

   string mesaj_reply;
   string mesaj_send;
   create_mesaje(args, mesaj_reply, mesaj_send, 2 ,msgrasp);

   printf("msgreply=#%s#\n,msgsend=#%s#\n", mesaj_reply.c_str(), mesaj_send.c_str());

   if (mesaj_reply.size() == 0 || mesaj_send.size() == 0)
   {
      msgrasp = "Eroare la formarea mesajelor";
      return;
   }

   int id_mesaj = get_id_mesaj(mesaj_reply, id, id_user, msgrasp);

   if (id_mesaj == -1)
   {
      return;
   }

   if (id_mesaj == -2)
   {
      msgrasp = "Nu exista acest mesaj";
      printf("mesajul:%s\n", mesaj_reply.c_str());
      return;
   }

   string msg = " <";
   msg += "replied ";
   msg += "\"";
   msg += mesaj_reply;
   msg += "\"";
   msg += '>';
   msg.push_back(' ');
   msg += mesaj_send;
   msg.push_back(' ');

   vector<string> args_send;
   args_send.push_back("send");
   user.push_back(':');
   args_send.push_back(user);
   args_send.push_back(msg);

   msgrasp = "Replied trimis";

   for (auto arg : args)
   {
      printf("mesajul trimis:#%s#\n", arg.c_str());
   }

   fn_send(args_send, msgrasp, id);
}