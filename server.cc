/**
 * Joshua Elliott
 * server.cc
 * CS360 - Lab 4 - Web Server
 */

#include "server.h"

using namespace std;

Server::Server(int _port, Config & _config, bool _debug)
      : port(_port), config(_config), debug(_debug) { 

  setup();
  if(debug) cout << "Server listening on port " << port << endl;
}

Server::~Server() {}

// void Server::start() {
//   cout << "You can't actually start this yet." << endl;
// }

// int main(int argc, char **argv) {
void Server::setup() {

  // setup socket address structure
  memset(&server,0,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = INADDR_ANY;

  // create socket
  s = socket(PF_INET,SOCK_STREAM,0);
  if (!s) {
    perror("socket");
    exit(-1);
  }

  // set socket to immediately reuse port when the application closes
  opt = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(-1);
  }

  // call bind to associate the socket with our local address and
  // port
  if (bind(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
    perror("bind");
    exit(-1);
  }

  // convert the socket to listen for incoming connections
  if (listen(s,SOMAXCONN) < 0) {
    perror("listen");
    exit(-1);
  }

  clientlen = sizeof(client);
  timeout = atoi(config.parameter("timeout").c_str());
}

void Server::start() {

  int epfd = epoll_create(1);

  // add listening socket to poller
  static struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = s;
  epoll_ctl(epfd, EPOLL_CTL_ADD, s, &ev);

  while (1) {
    // do poll
    struct epoll_event events[MAX_EVENTS];
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, 1);
    if (nfds < 0) {
      perror("epoll");
      exit(EXIT_FAILURE);
    }
    if (nfds == 0)
      continue;

    // handle sockets that are ready
    for(int i = 0; i < nfds; i++) {

      int fd = events[i].data.fd;
      if (fd == s) {
        c = accept(s,(struct sockaddr *)&client,&clientlen);
        if (c < 0) {
          perror("accept");
          break;
        }
        // add new client to poller
        ev.events = EPOLLIN;
        ev.data.fd = c;
        epoll_ctl(epfd, EPOLL_CTL_ADD, c, &ev);
      } else {
        // handle client
        bool result = h.handle(fd);
        if (!result) {
          // socket closed, so remove it from poller
          ev.events = EPOLLIN;
          ev.data.fd = fd;
          epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
          close(fd);
        }
      }
    }
  }
  close(s);
}
