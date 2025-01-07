#include <functii/baze_de_date.h>

using namespace std;

vector<string> prelucram_comanda(char input[])
{
    vector<string> argumente;
    string arg = "";
    for (size_t i = 0; i < strlen(input); i++)
    {
        if (input[i] == ' ')
        {
            argumente.push_back(arg);
            arg = "";
        }
        else
        {
            arg.push_back(input[i]);
        }
    }

    if (arg.length() != 0)
    {
        argumente.push_back(arg);
    }

    return argumente;
}

void BazaDeDateGrupuri()
{
   sqlite3 *bd;
   char *ErrMsg = nullptr;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      printf("[bdGrupuri] Eroare la deschiderea bazei de date.");
      return;
   }

   const char *sqlGrupuri = R"(
    CREATE TABLE IF NOT EXISTS grupuri (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        nume TEXT NOT NULL UNIQUE,
        id_creator INTEGER NOT NULL,
        FOREIGN KEY (id_creator) REFERENCES users(id)
    );
)";

   rc = sqlite3_exec(bd, sqlGrupuri, nullptr, nullptr, &ErrMsg);
   if (rc != SQLITE_OK)
   {
      printf("[bdGrupuri] Eroare comanda pentru sqlGrupuri: %s\n", ErrMsg);
      sqlite3_free(ErrMsg);
   }

   const char *sqlMembriGrup = R"(
        CREATE TABLE IF NOT EXISTS membri_grup (
            id_grup INTEGER NOT NULL,
            id_user INTEGER NOT NULL,
            rol TEXT DEFAULT 'membru', 
            PRIMARY KEY (id_grup, id_user),
            FOREIGN KEY (id_grup) REFERENCES grupuri(id),
            FOREIGN KEY (id_user) REFERENCES users(id)
        );
    )";

   rc = sqlite3_exec(bd, sqlMembriGrup, nullptr, nullptr, &ErrMsg);
   if (rc != SQLITE_OK)
   {
      printf("[bdMembriGrup] Eroare la sqlMembriGrup: %s\n", ErrMsg);
      sqlite3_free(ErrMsg);
   }

   const char *sqlMesajeGrup = R"(
        CREATE TABLE IF NOT EXISTS mesaje_grup (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_grup INTEGER NOT NULL,
            id_sender INTEGER NOT NULL,
            mesaj TEXT NOT NULL,
            data_mesaj DATETIME NOT NULL,
            FOREIGN KEY (id_grup) REFERENCES grupuri(id),
            FOREIGN KEY (id_sender) REFERENCES users(id)
        );
    )";

   rc = sqlite3_exec(bd, sqlMesajeGrup, nullptr, nullptr, &ErrMsg);
   if (rc != SQLITE_OK)
   {
      printf("[bdMesajeGrup] Eroare: %s\n", ErrMsg);
      sqlite3_free(ErrMsg);
   }

   sqlite3_close(bd);
}

void BazaDeDateMesaje()
{
   sqlite3 *bd;
   char *ErrMsg = nullptr;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      printf("[bdMesaje] Eroare la deschiderea bazei de date.\nEroarea: %s\n", sqlite3_errmsg(bd));
      return;
   }

   const char *sql = R"(
    CREATE TABLE IF NOT EXISTS mesaje (
        id INTEGER PRIMARY KEY AUTOINCREMENT,   
        id_sender INTEGER NOT NULL,  
        id_receive INTEGER NOT NULL,     
        mesaj TEXT NOT NULL,                   
        status_mesaj TEXT DEFAULT 'necitit', 
        status_receiver TEXT DEFAULT 'online',          
        notificare_trimisa INTEGER DEFAULT 0,
        data_mesaj DATETIME NOT NULL, 
        FOREIGN KEY(id_sender) REFERENCES users(id), 
        FOREIGN KEY(id_receive) REFERENCES users(id)
    );
)";

   rc = sqlite3_exec(bd, sql, 0, 0, &ErrMsg);
   if (rc != SQLITE_OK)
   {
      printf("[bdMesaje] Eroare: %s\n", ErrMsg);
      sqlite3_free(ErrMsg);
   }

   sqlite3_close(bd);
}

void BazaDeDateUsers()
{
   sqlite3 *bd;
   char *ErrMsg = nullptr;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      printf("[bdUsers]Eroare la deschiderea bazei de date.\nEroarea: %s\n", sqlite3_errmsg(bd));
      return;
   }

   const char *sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            status TEXT DEFAULT 'offline'
        );
    )";

   rc = sqlite3_exec(bd, sql, 0, 0, &ErrMsg);
   if (rc != SQLITE_OK)
   {
      printf("[bdUsers]Eroare:%s\n", ErrMsg);
      sqlite3_free(ErrMsg);
   }

   sqlite3_close(bd);
}

string trim(const string &str)
{
   size_t start = str.find_first_not_of(' ');
   size_t end = str.find_last_not_of(' ');
   return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

int get_id_by_user(string &user, string &msgrasp)
{

   sqlite3_stmt *stmt;
   sqlite3 *bd;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "Eroare la deschiderea bazei de date.";
      perror("[send]Eroare la deschiderea bazei de date: ");
      return -1;
   }

   string comanda1 = "SELECT id FROM users WHERE username = ?";
   rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "Eroare la pregatirea comenzii SQL de cautare id-user";
      printf("[send]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
      sqlite3_close(bd);
      return -1;
   }

   sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      int id = sqlite3_column_int(stmt, 0);
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return id;
   }
   else
   {
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return 0;
   }
}

int get_status_by_id(int id, string &msgrasp)
{

   sqlite3_stmt *stmt;
   sqlite3 *bd;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "Eroare la deschiderea bazei de date.";
      perror("[status_id]]Eroare la deschiderea bazei de date: ");
      return -1;
   }

   string comanda1 = "SELECT status FROM users WHERE id = ?";
   rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
      printf("[status_id]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
      sqlite3_close(bd);
      return -1;
   }

   sqlite3_bind_int(stmt, 1, id);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      const char *status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

      if (strcmp(status, "online") == 0)
      {
         sqlite3_finalize(stmt);
         sqlite3_close(bd);
         return 1;
      }
      else
      {
         sqlite3_finalize(stmt);
         sqlite3_close(bd);
         return 0;
      }
   }
   else
   {
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return -1;
   }
}

string get_username_by_id(int id, string msgrasp)
{
   sqlite3 *bd;
   sqlite3_stmt *stmt;
   std::string username = "";

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "[GetUsernameById]Eroare la deschiderea bazei de date!";
      return "";
   }

   const char *sql = R"(
        SELECT username
        FROM users
        WHERE id = ?;
    )";

   rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "[GetUsernameById]Eroare la pregătirea comenzii SQL!";
      sqlite3_close(bd);
      return "";
   }

   sqlite3_bind_int(stmt, 1, id);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
   }

   sqlite3_finalize(stmt);
   sqlite3_close(bd);

   return username;
}

int get_id_mesaj(string mesaj, int id, int id_user, string &msgrasp)
{

   sqlite3_stmt *stmt;
   sqlite3 *bd;

   mesaj = trim(mesaj);

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "Eroare la deschiderea bazei de date.";
      perror("[id_mesaj]]Eroare la deschiderea bazei de date: ");
      return -1;
   }

   string comanda1 = "SELECT id FROM mesaje WHERE mesaj = ? AND id_sender = ?";
   rc = sqlite3_prepare_v2(bd, comanda1.c_str(), -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "Eroare la pregatirea comenzii SQL de verificare a utilizatorului";
      printf("[id_mesaj]Eroare la pregatirea comenzii de verificare: %s\n", sqlite3_errmsg(bd));
      sqlite3_close(bd);
      return -1;
   }

   sqlite3_bind_text(stmt, 1, mesaj.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 2, id_user);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      int id = sqlite3_column_int(stmt, 0);
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return id;
   }
   else
   {
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return -2;
   }
}

int get_id_grup(string groupName, string &msgrasp)
{
   sqlite3 *bd;
   sqlite3_stmt *stmt;
   int idGrup = 0;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "[GetIdGroup] Eroare la deschiderea bazei de date";
      return -1;
   }

   const char *sql = "SELECT id FROM grupuri WHERE nume = ?;";
   rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      sqlite3_close(bd);
      msgrasp = "[GetIdGroup] Eroare la pregatirea comenzii sql";
      return -1;
   }

   sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, SQLITE_STATIC);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      idGrup = sqlite3_column_int(stmt, 0);
   }

   sqlite3_finalize(stmt);
   sqlite3_close(bd);

   return idGrup;
}

string get_status_membru(string groupName, string &msgrasp, int idUser)
{
   sqlite3 *bd;
   sqlite3_stmt *stmt;
   string rol;

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "[GetStatusMembru]Eroare la deschiderea bazei de date.";
      return "Err";
   }

   int idGroup = get_id_grup(groupName, msgrasp);
   if (idGroup == -1)
   {
      return "Err";
   }

   const char *sql = "SELECT rol FROM membri_grup WHERE id_grup = ? AND id_user = ?;";
   rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      sqlite3_close(bd);
      msgrasp = "[GetStatusMembry]Eroare la pregătirea comenzii SQL.";
      return "Err";
   }

   sqlite3_bind_int(stmt, 1, idGroup);
   sqlite3_bind_int(stmt, 2, idUser);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      const unsigned char *role = sqlite3_column_text(stmt, 0);
      rol = role ? reinterpret_cast<const char *>(role) : "Necunoscut";
   }
   else
   {
      msgrasp = "Utilizatorul nu este membru al grupului.";
      return "Err";
   }

   sqlite3_finalize(stmt);
   sqlite3_close(bd);

   return rol;
}

string get_admin_user(int groupId, string &msgrasp)
{
   sqlite3 *bd;
   sqlite3_stmt *stmt;
   std::string adminUsername = "";

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "[GetAdminUser]Eroare la deschiderea bazei de date!";
      return "";
   }

   const char *sql = R"(
        SELECT u.username
        FROM users u
        INNER JOIN membri_grup mg ON u.id = mg.id_user
        WHERE mg.id_grup = ? AND mg.rol = 'admin';
    )";

   rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "Eroare la pregatirea comenzii SQL!";
      sqlite3_close(bd);
      return "";
   }

   sqlite3_bind_int(stmt, 1, groupId);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      adminUsername = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
   }

   sqlite3_finalize(stmt);
   sqlite3_close(bd);

   return adminUsername;
}

int get_id_mesaj_group(const string &mesaj, int group_id, int user_id, string &msgrasp)
{
   sqlite3 *bd;
   sqlite3_stmt *stmt;

   string mesaj_trim = trim(mesaj);

   int rc = sqlite3_open("bd/users.db", &bd);
   if (rc)
   {
      msgrasp = "[get_id_mesaj_group] Eroare la deschiderea bazei de date.";
      perror("[get_id_mesaj_group] Eroare la deschiderea bazei de date:");
      return -1;
   }

   const char *sql = R"(
        SELECT id 
        FROM mesaje_grup 
        WHERE mesaj = ? AND id_grup = ? AND id_sender = ?
    )";

   rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      msgrasp = "[get_id_mesaj_group] Eroare la pregătirea comenzii SQL.";
      printf("[get_id_mesaj_group] Eroare SQL: %s\n", sqlite3_errmsg(bd));
      sqlite3_close(bd);
      return -1;
   }

   sqlite3_bind_text(stmt, 1, mesaj_trim.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 2, group_id);
   sqlite3_bind_int(stmt, 3, user_id);

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      int id_mesaj = sqlite3_column_int(stmt, 0);
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return id_mesaj;
   }
   else if (rc == SQLITE_DONE)
   {
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return 0;
   }
   else
   {
      msgrasp = "[get_id_mesaj_group] Eroare la executarea interogării SQL.";
      sqlite3_finalize(stmt);
      sqlite3_close(bd);
      return -1;
   }
}
