#pragma once

#ifndef MY_LIB_H
#define MY_LIB_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>

#include "const.h"
#include "functii/exit.h"
#include "functii/login.h"
#include "functii/logout.h"
#include "functii/help.h"
#include "functii/history.h"
#include "functii/reply.h"
#include "functii/send.h"
#include "functii/users.h"
#include "functii/register.h"
#include "functii/baze_de_date.h"
#include "functii/offlinemessage.h"
#include "functii/notificariMesaje.h"
#include "functii/grup.h"

#include "comunic/comunicare.h"

#endif 
