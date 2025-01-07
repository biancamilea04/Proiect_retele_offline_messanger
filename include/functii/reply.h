#pragma once

#include "biblioteci.h"
#include "baze_de_date.h"
#include "send.h"

using namespace std;

void create_mesaje(vector<string> args, string &mesaj_reply, string &mesaj_send, int start ,string &msgrasp);

void fn_reply(vector<string> args, string &msgrasp, int id);