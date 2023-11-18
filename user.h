#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

using namespace std;

class User
{
  int m_id;
  int m_socket;
  string m_name;
  string m_password;
public:
  User(int id, int socket) : m_id(id), m_socket(socket) {}
  
  int getId() const { return m_id; }
  int getSocket() const { return m_socket; }
  string getName() const { return m_name; }
  string getPassword() const { return m_password; }

  void setName(const string& name) { m_name = name; }
  void setPassword(const string& password) { m_password = password; }

  void send(const string& msg);
};

class UserMgr
{
  unordered_map<int, shared_ptr<User>> m_users;
  mutex m_mutex;
public:
  void addUser(shared_ptr<User> user);
  void removeUser(int id);
  shared_ptr<User> getUser(int id);

  void broadcast(const string& msg);
};

#endif