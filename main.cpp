#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <string.h>
#include <string>
#include <iostream>
#include <thread>

#include "global.h"
#include "msgProcess.h"


using namespace std;

#define MAX_EVENTS 10

void make_socket_non_blocking(int sfd) {
  int flags = fcntl(sfd, F_GETFL, 0);
  fcntl(sfd, F_SETFL, flags | O_NONBLOCK);
}

struct Skt{
  int m_fd;
  Skt(int fd):m_fd(fd){}
  ~Skt(){
    close(m_fd);
  }
};

int main() {
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  Skt skt(sfd);
  make_socket_non_blocking(sfd);

  int opt = 1;
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  MsgProcess msgProcessor;
  thread msgProcessThread(&MsgProcess::run, &msgProcessor);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sfd, (struct sockaddr*)&addr, sizeof addr);

  listen(sfd, SOMAXCONN);

  int efd = epoll_create1(0);
  struct epoll_event event;
  event.data.fd = sfd;
  event.events = EPOLLIN;
  epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);

  while (1) {
    struct epoll_event events[MAX_EVENTS];
    int n = epoll_wait(efd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++) {
      if (events[i].data.fd == sfd) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof client_addr;
        int client_fd = accept(sfd, (struct sockaddr*)&client_addr, &client_len);
        //client ip address
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof ip);
        cout << "client ip: " << ip << endl;

        make_socket_non_blocking(client_fd);
        event.data.fd = client_fd;
        event.events = EPOLLIN;
        epoll_ctl(efd, EPOLL_CTL_ADD, client_fd, &event);
      } else {
        char buf[1024];
        int len = read(events[i].data.fd, buf, sizeof buf);

        if (len == 0) {
          close(events[i].data.fd);
        } else {
          string s(buf, len);
          g_msgQ.Emplace(Msg(events[i].data.fd, s));
        }
      }
    }
  }

  return 0;
}