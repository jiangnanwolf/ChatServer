#ifndef GLOBAL_H
#define GLOBAL_H

#include "q.h"
#include "msg.h"
#include "user.h"

extern ThreadSafeQueue<Msg> g_msgQ;

extern UserMgr g_userMgr;

#endif