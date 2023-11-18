#include "user.h"
#include <sys/socket.h>

void User::send(const string& msg)
{
  ::send(m_socket, msg.c_str(), msg.length(), 0);
}

void UserMgr::addUser(shared_ptr<User> user)
{
  lock_guard<mutex> lock(m_mutex);
  m_users[user->getId()] = user;
}

void UserMgr::removeUser(int id)
{
  lock_guard<mutex> lock(m_mutex);
  m_users.erase(id);
}

shared_ptr<User> UserMgr::getUser(int id)
{
  lock_guard<mutex> lock(m_mutex);
  auto it = m_users.find(id);
  if(it == m_users.end())
    return nullptr;
  return it->second;
}

void UserMgr::broadcast(const string& msg)
{
  lock_guard<mutex> lock(m_mutex);
  for (auto& user : m_users) {
    user.second->send(msg);
  }
}