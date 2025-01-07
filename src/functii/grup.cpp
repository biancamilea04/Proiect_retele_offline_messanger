#include "functii/grup.h"

void fn_createGrup(vector<string> args, string &msgrasp, int &id)
{
    sqlite3 *bd;

    if (id == 0)
    {
        msgrasp = "[createGroup]Comanda invalida. Va rugam sa va logati!";
        return;
    }

    if (args.size() != 2)
    {
        msgrasp = "[createGroup]Comanda invalida. Incearca \"create [GROUPNAME]\".";
        return;
    }

    string numeGrup = trim(args[1]);

    if (numeGrup.empty())
    {
        msgrasp = "[createGroup]Numele grupului nu este valid.";
        return;
    }

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[createGroup]Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sqlInsertGrup = "INSERT INTO grupuri (nume, id_creator) VALUES (?, ?);";
    sqlite3_stmt *stmtInsertGrup;
    rc = sqlite3_prepare_v2(bd, sqlInsertGrup, -1, &stmtInsertGrup, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "[createGroup]Eroare la pregatirea comenzii SQL pentru crearea grupului.";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_text(stmtInsertGrup, 1, numeGrup.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmtInsertGrup, 2, id);

    rc = sqlite3_step(stmtInsertGrup);
    if (rc != SQLITE_DONE)
    {
        msgrasp = "[createGroup]Eroare.";
        sqlite3_finalize(stmtInsertGrup);
        sqlite3_close(bd);
        return;
    }

    sqlite3_finalize(stmtInsertGrup);

    int groupId = sqlite3_last_insert_rowid(bd);

    const char *sqlInsertMembru = "INSERT INTO membri_grup (id_grup, id_user, rol) VALUES (?, ?, 'admin');";
    sqlite3_stmt *stmtInsertMembru;
    rc = sqlite3_prepare_v2(bd, sqlInsertMembru, -1, &stmtInsertMembru, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "[createGroup]Eroare la pregatirea comenzii SQL pentru adaugarea utilizatorului ca admin.";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmtInsertMembru, 1, groupId);
    sqlite3_bind_int(stmtInsertMembru, 2, id);

    rc = sqlite3_step(stmtInsertMembru);
    if (rc != SQLITE_DONE)
    {
        msgrasp = "[createGroup]Eroare la adăugarea utilizatorului ca admin.";
        sqlite3_finalize(stmtInsertMembru);
        sqlite3_close(bd);
        return;
    }

    sqlite3_finalize(stmtInsertMembru);
    sqlite3_close(bd);

    msgrasp = "Grupul a fost creat cu succes.";
}

bool IsUserInGroup(int id_grup, int id_user)
{
    sqlite3 *bd;
    sqlite3_stmt *stmt;
    bool isInGroup = false;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        return false;
    }

    const char *sql = R"(
        SELECT 1
        FROM membri_grup
        WHERE id_grup = ? AND id_user = ?;
    )";

    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(bd);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id_grup);
    sqlite3_bind_int(stmt, 2, id_user);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        isInGroup = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);

    return isInGroup;
}

void fn_addMembers(vector<string> args, string &msgrasp, int &idUser)
{

    if (idUser == 0)
    {
        msgrasp = "[addMembers]Comanda invalida. Va rugam sa va logati!";
        return;
    }

    if (args.size() != 3)
    {
        msgrasp = "[addMembers]Comanda invalida. Incearca \"add [GROUPNAME] [USERNAME]\".";
        return;
    }

    string groupName = args[1];
    string userAdd = args[2];
    int idGroup = get_id_grup(groupName, msgrasp);

    if (idGroup == -1)
    {
        return;
    }

    if (idGroup == 0)
    {
        msgrasp = "Nu exista grupul ";
        msgrasp += groupName;
        msgrasp += ".";
        return;
    }

    if (IsUserInGroup(idGroup, idUser) == false)
    {
        msgrasp += "Nu esti in acest grup";
        return;
    }

    string rol = get_status_membru(groupName, msgrasp, idUser);
    if (rol == "Err")
    {
        return;
    }

    if (rol != "admin")
    {
        string usernameAdmin = get_admin_user(idGroup, msgrasp);
        if (usernameAdmin == "")
        {
            return;
        }

        msgrasp = "Trebuie sa fii admin ca sa adaugi membrii in grup.\n";
        msgrasp += "Adminul grupului: ";
        msgrasp += usernameAdmin;
        return;
    }

    int idUserAdd = get_id_by_user(userAdd, msgrasp);

    if (IsUserInGroup(idGroup, idUserAdd))
    {
        msgrasp = "Userul este deja in grup";
        return;
    }

    sqlite3 *bd;

    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[addMembers]Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sql = "INSERT INTO membri_grup (id_grup, id_user) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "Eroare la pregătirea comenzii SQL pentru adăugarea unui membru.";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, idGroup);
    sqlite3_bind_int(stmt, 2, idUserAdd);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        msgrasp = "Eroare: Utilizatorul este deja membru al grupului.";
        sqlite3_finalize(stmt);
        sqlite3_close(bd);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
    msgrasp = "Utilizatorul a fost adăugat în grup.";
}

void fn_group(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "[group] Comanda invalida. Va rugam sa va logati!";
        return;
    }

    if (args.size() != 1)
    {
        msgrasp = "[group] Comanda invalida. Incearca group.";
        return;
    }

    sqlite3 *bd;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[group] Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sql = R"(
        SELECT g.nume
        FROM grupuri g
        INNER JOIN membri_grup mg ON g.id = mg.id_grup
        WHERE mg.id_user = ?;
    )";

    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "[group] Eroare la pregatirea comenzii SQL!";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    bool hasGroups = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        hasGroups = true;
        const char *groupName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        msgrasp += "\n";
        msgrasp += groupName;
    }

    if (!hasGroups)
    {
        msgrasp = "Nu esti in niciun grup.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}

void fn_groupMembers(vector<string> args, string &msgrasp, int id)
{

    if (id == 0)
    {
        msgrasp = "[groupMembers]Comanda invalida. Va rugam sa va logati!";
        return;
    }

    if (args.size() != 2)
    {
        msgrasp = "[group]Comanda invalida. Incearca \"groupMembers [GROUPNAME]\".";
        return;
    }

    string groupName = args[1];
    int idGroup = get_id_grup(groupName, msgrasp);

    if (idGroup == -1)
    {
        return;
    }

    if (idGroup == 0)
    {
        msgrasp = "Nu exista grupul ";
        msgrasp += groupName;
        msgrasp += ".";
        return;
    }

    sqlite3 *bd;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[groupMembers]Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sql = R"(
        SELECT users.username, membri_grup.rol
        FROM membri_grup
        INNER JOIN grupuri ON membri_grup.id_grup = grupuri.id
        INNER JOIN users ON membri_grup.id_user = users.id
        WHERE grupuri.nume = ?;
    )";

    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(bd);
        msgrasp = "[groupMembers]Eroare la pregătirea comenzii SQL.";
    }
    sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, SQLITE_STATIC);

    msgrasp = groupName += ":";

    rc = sqlite3_step(stmt);
    bool members = false;
    while (rc == SQLITE_ROW)
    {
        members = true;

        const char *username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char *rol = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        msgrasp += "\n";
        msgrasp += username;
        msgrasp += " (";
        msgrasp += rol;
        msgrasp += ")";

        rc = sqlite3_step(stmt);
    }

    if (!members)
    {
        msgrasp += "Nu există membri în acest grup.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}

void fn_sendGroup(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "[sendGroup] Comanda invalida. Utilizator nelogat.";
        return;
    }

    if (args.size() < 3 || args[1][args[1].size() - 1] != ':')
    {
        msgrasp = "[sendGroup] Comanda invalida. Incearca \"sendGroup [GROUPNAME]: [MESAJ]\"";
        return;
    }

    string groupName = args[1];
    groupName.pop_back();

    string mesaj;
    for (size_t i = 2; i < args.size(); i++)
    {
        mesaj += args[i] + " ";
    }

    mesaj = trim(mesaj);

    if (mesaj.empty())
    {
        msgrasp = "Mesajul este gol.";
        return;
    }

    int groupId = get_id_grup(groupName, msgrasp);
    if (groupId == -1)
    {
        return;
    }
    if (groupId == 0)
    {
        msgrasp = "Grupul nu exista.";
        return;
    }

    sqlite3 *bd;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[sendGroup] Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sql = R"(
        INSERT INTO mesaje_grup (id_grup, id_sender, mesaj, data_mesaj)
        VALUES (?, ?, ?, datetime('now'));
    )";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "[sendGroup] Eroare la pregatirea comenzii SQL.";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_text(stmt, 3, mesaj.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        msgrasp = "[sendGroup] Eroare la inserarea mesajului în baza de date.";
    }
    else
    {
        msgrasp = "Mesaj trimis cu succes.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}

void fn_historyGroup(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "[historyGroup] Comanda invalida. Utilizator nelogat.";
        return;
    }

    if (args.size() != 2)
    {
        msgrasp = "[historyGroup] Comanda invalida. Incearca \"historyGroup [GROUPNAME]\"";
        return;
    }

    string groupName = trim(args[1]);

    int groupId = get_id_grup(groupName, msgrasp);
    if (groupId == -1)
    {
        return;
    }
    if (groupId == 0)
    {
        msgrasp = "Grupul nu există.";
        return;
    }

    sqlite3 *bd;
    int rc = sqlite3_open("bd/users.db", &bd);
    if (rc)
    {
        msgrasp = "[historyGroup] Eroare la deschiderea bazei de date.";
        return;
    }

    const char *sql = R"(
        SELECT u.username, m.mesaj
        FROM mesaje_grup m
        INNER JOIN users u ON m.id_sender = u.id
        WHERE m.id_grup = ?
        ORDER BY m.data_mesaj ASC;
    )";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        msgrasp = "[historyGroup] Eroare la pregatirea comenzii SQL.";
        sqlite3_close(bd);
        return;
    }

    sqlite3_bind_int(stmt, 1, groupId);

    string history = groupName + ":";

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        string sender = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        string message = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        history += "\n" + sender + "> " + message ;
    }

    if (rc != SQLITE_DONE)
    {
        msgrasp = "[historyGroup] Eroare la obtinerea mesajelor.";
    }
    else if (history == groupName + ":")
    {
        msgrasp = history + "Nu exista mesaje in acest grup.";
    }
    else
    {
        msgrasp = history;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd);
}

void fn_replyGroup(vector<string> args, string &msgrasp, int id)
{
    if (id == 0)
    {
        msgrasp = "[replyGroup] Comanda invalida. Utilizator nelogat!";
        return;
    }

    if (args.size() < 5)
    {
        msgrasp = "[replyGroup] Comanda invalida. Incearca \"replyGroup [GROUPNAME] [USERNAME] [MESAJ_REPLY]: [MESAJ]\"";
        return;
    }

    string groupName = args[1];
    string username = args[2];

    int groupId = get_id_grup(groupName, msgrasp);
    if (groupId == -1)
    {
        return;
    }
    if (groupId == 0)
    {
        msgrasp = "Grupul \"" + groupName + "\" nu exista.";
        return;
    }

    if (!IsUserInGroup(groupId, id))
    {
        msgrasp = "Nu esti membru al acestui grup.";
        return;
    }

    int userId = get_id_by_user(username, msgrasp);
    if (userId == -1)
    {
        return;
    }
    if (userId == 0)
    {
        msgrasp = "Utilizatorul \"" + username + "\" nu exista.";
        return;
    }

    if (!IsUserInGroup(groupId, userId))
    {
        msgrasp = "Utilizatorul" + username + " nu este membru al acestui grup.";
        return;
    }

    string mesaj_reply, mesaj_send;
    create_mesaje(args, mesaj_reply, mesaj_send, 3, msgrasp);

    if (mesaj_reply.empty() || mesaj_send.empty())
    {
        msgrasp = "[replyGroup] Eroare la procesarea mesajelor.";
        return;
    }

    int id_mesaj = get_id_mesaj_group(mesaj_reply, groupId, userId, msgrasp);
    if (id_mesaj == -1)
    {
        msgrasp = "[replyGroup] Eroare la găsirea mesajului.";
        return;
    }
    if (id_mesaj == 0)
    {
        msgrasp = "[replyGroup] Mesajul \"" + mesaj_reply + "\" nu a fost trimis de utilizatorul \"" + username + "\".";
        return;
    }

    string msg = "<replied " +  username + ":\"" + mesaj_reply + "\"> " + mesaj_send;

    vector<string> args_send;
    args_send.push_back("sendGroup");
    string name = groupName + ":"; 
    args_send.push_back(name);
    args_send.push_back(msg);

    fn_sendGroup(args_send,msgrasp,id);
}
