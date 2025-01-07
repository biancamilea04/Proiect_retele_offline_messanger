#pragma once

#include "biblioteci.h"

using namespace std;

vector<string> prelucram_comanda(char input[]);

void BazaDeDateGrupuri();

void BazaDeDateMesaje();

void BazaDeDateUsers();

string trim(const string &str);

int get_id_by_user(string &user, string &msgrasp);

int get_status_by_id(int id, string &msgrasp);

string get_username_by_id(int id, string msgrasp);

int get_id_mesaj(string mesaj, int id, int id_user, string &msgrasp);

int get_id_grup(string groupName, string &msgrasp);

string get_status_membru(string groupName, string &msgrasp, int idUser);

string get_admin_user(int groupId, string &msgrasp);

int get_id_mesaj_group(const string &mesaj, int group_id, int user_id, string &msgrasp);