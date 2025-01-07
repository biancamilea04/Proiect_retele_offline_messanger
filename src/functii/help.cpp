#include "functii/help.h"

using namespace std;

void fn_help(vector<string> args, string &msgrasp)
{
   msgrasp = "~H E L P~\n";
   msgrasp += "Bine ai venit utilizatorule. Acesta este un meniu pentru comenzile programului.\n\n";

   msgrasp += "[EXIT] Pentru a iesi foloseste comanda \"exit\".\n";
   msgrasp += "[REGISTER] Pentru a te inregistra in aplicatie foloseste comanda \"register <username> <parola> <parola_confirmata>\".\n";
   msgrasp += "[LOGIN] Pentru a te conecta la aplicatie si a accesa functionalitatile foloseste comanda \"login [username] [parola]\".\n";
   msgrasp += "[LOGOUT] Pentru a te deconecta din aplicatie foloseste comanda \"logout\".\n\n";

   msgrasp += "Urmatoarele comenzi nu vor putea fi utilizate daca nu esti logat in aplicatie.\n";
   msgrasp += "[USERS] Pentru a vedea utilizatorii logati utilizeaza comanda \"users\".\n";
   msgrasp += "[SEND] Pentru a trimite un mesaj unui utilizator foloseste comanda \"send [username] [mesaj]\".\n";
   msgrasp += "[REPLY] Pentru a raspunde la un mesaj primit foloseste comanda \"reply [username] [mesaj_reply]: [mesaj]\".\n";
   msgrasp += "[HISTORY] Pentru a vizualiza istoricul mesajelor cu un utilizator foloseste comanda \"history [username]\".\n";
   msgrasp += "[OFFLINE] Pentru a vizualiza mesajele necitite cand nu esti logat foloseste comanda \"ShowUnreadMessage [username]\".\n\n";

   msgrasp += "Urmatoarele comenzi sunt pentru gestionarea grupurilor:\n";
   msgrasp += "[CREATE] Pentru a crea un grup foloseste comanda \"create <group_name>\".\n";
   msgrasp += "[ADD] Pentru a adauga un utilizator intr-un grup foloseste comanda \"add [group_name] [username]\".\n";
   msgrasp += "[GROUP] Pentru a vizualiza grupurile din care faci parte foloseste comanda \"group\".\n";
   msgrasp += "[GROUPMEMBERS] Pentru a vedea membrii unui grup foloseste comanda \"groupMembers [group_name]\".\n";
   msgrasp += "[SENDGROUP] Pentru a trimite un mesaj intr-un grup foloseste comanda \"sendGroup [group_name] [mesaj]\".\n";
   msgrasp += "[HISTORYGROUP] Pentru a vizualiza istoricul mesajelor dintr-un grup foloseste comanda \"historyGroup [group_name]\".\n";
   msgrasp += "[REPLYGROUP] Pentru a raspunde la un mesaj dintr-un grup foloseste comanda \"replyGroup [group_name] [username] [mesaj_reply]: [mesaj]\".\n\n";

   msgrasp += "[HELP] Daca ai nevoie de ajutor foloseste comanda \"help\"";
   msgrasp += "\n~ Sfarsit ~\n";
}
