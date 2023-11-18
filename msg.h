#ifndef MSG_H
#define MSG_H

#include <string>

using namespace std;

struct Msg
{
  int m_socket;
  int m_senderId;
  int m_receiverId;
  string m_msg;

public:
  Msg() : m_socket(-1), m_senderId(-1), m_receiverId(-1) {}
  Msg(int socket, const string& msg)
    : m_socket(socket), m_msg(msg) {}
  
};

/*

senderId:receiverId:msg

*/

#endif