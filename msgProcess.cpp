#include <iostream>

#include "msgProcess.h"
#include "global.h"

using namespace std;

void MsgProcess::run()
{
  while (1) {
    Msg msg;
    if(!g_msgQ.WaitAndPop(msg))
      break;  // quit
    int senderId = msg.m_msg[0]-'0';
    int receiverId = msg.m_msg[1]-'0';

    auto user = g_userMgr.getUser(senderId);
    if(!user)
      g_userMgr.addUser(make_shared<User>(senderId, msg.m_socket));

    if (receiverId == 0) {
      // broadcast
      broadcast(msg.m_msg);
    } else {
      // unicast
      send(receiverId, msg.m_msg);
    }

  }
}

void MsgProcess::broadcast(const string& msg)
{
  g_userMgr.broadcast(msg);
}

void MsgProcess::send(int receiverId, const string& msg)
{
  auto user = g_userMgr.getUser(receiverId);
  if (user)
    user->send(msg);
}