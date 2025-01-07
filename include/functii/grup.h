#pragma once
#include "biblioteci.h"

using namespace std;

void fn_createGrup(vector<string> args, string &msgrasp, int &id);

bool IsUserInGroup(int id_grup, int id_user);

void fn_addMembers(vector<string> args, string &msgrasp, int &idUser);

void fn_group(vector<string> args, string &msgrasp, int id);

void fn_groupMembers(vector<string> args, string &msgrasp, int id);

void fn_sendGroup(vector<string> args, string &msgrasp, int id);

void fn_historyGroup(vector<string> args, string &msgrasp, int id);

void fn_replyGroup(vector<string> args, string &msgrasp, int id);