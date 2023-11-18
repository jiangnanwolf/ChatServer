#ifndef MSGPROCESS_H
#define MSGPROCESS_H

#include <string>

using namespace std;

struct MsgProcess
{
  void run();
  void broadcast(const string& msg);
  void send(int receiverId, const string& msg);
};


#endif