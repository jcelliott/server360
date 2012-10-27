/**
 * Joshua Elliott
 * server.cc
 * CS360 - Lab 4 - Web Server
 */

#include "server.h"

using namespace std;

Server::Server(int _port, Config & _config, Logger & _log)
      : port(_port), config(_config), log(_log) { 

  setup();
  // if(debug) cout << "Server listening on port " << port << endl;
  log << Logger::info << "Server listening on port " << port << Logger::endl;
}

Server::~Server() {}

// void Server::start() {
//   cout << "You can't actually start this yet." << endl;
// }

// int main(int argc, char **argv) {
void Server::setup() {
  log << Logger::debug << "Server.setup()\n";

  // setup socket address structure
  log << Logger::debug << "setup socket address structure\n";
  memset(&server,0,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = INADDR_ANY;

  // create socket
  log <<Logger::debug << "create socket\n";
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

  // call bind to associate the socket with our local address and port
  log << Logger::debug << "bind socket\n";
  if (bind(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
    perror("bind");
    exit(-1);
  }

  // convert the socket to listen for incoming connections
  log << Logger::debug << "listen on socket\n";
  if (listen(s,SOMAXCONN) < 0) {
    perror("listen");
    exit(-1);
  }

  clientlen = sizeof(client);
  idleTimeout = atoi(config.parameter("timeout").c_str());
}

void Server::start() {
  log << Logger::debug << "Server.start()\n";

  int epfd = epoll_create(1);

  // add listening socket to poller
  log << Logger::debug << "Add listening socket to poller: " << s << Logger::endl;
  static struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = s;
  epoll_ctl(epfd, EPOLL_CTL_ADD, s, &ev);

  while (1) {
    // do poll
    struct epoll_event events[MAX_EVENTS];
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, 1000);
    if (nfds < 0) {
      perror("epoll");
      exit(EXIT_FAILURE);
    }

    // handle timeouts
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double current = ts.tv_sec + ts.tv_nsec/1000000000.0;
    // log << Logger::debug << "current time: " << current << Logger::endl;
    map<int, Handler*>::iterator it;
    vector<int> timeouts;
    for (it=handlers.begin(); it != handlers.end(); it++) {
      if (current - (*it).second->lastEvent() > idleTimeout) {
        timeouts.push_back((*it).first);
      }
    }
    for (int j=0; j < timeouts.size(); j++) {
      log << Logger::debug << "client timed out: " << timeouts[j] << Logger::endl;
      handlers.erase(timeouts[j]);
      close(timeouts[j]);
    }
    
    if (nfds == 0) {
      // timeout
      continue;
    }

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
        log << Logger::info << "client connected: " << c << Logger::endl;
        ev.events = EPOLLIN;
        ev.data.fd = c;
        // create a new handler for this client
        handlers[c] = new Handler(c, config, log);
        epoll_ctl(epfd, EPOLL_CTL_ADD, c, &ev);
      } else {
        // handle client
        // bool result = h.handle(fd);
        bool result = handlers[fd]->handle();
        if (!result) {
          // socket closed, so remove it from poller
          ev.events = EPOLLIN;
          ev.data.fd = fd;
          delete handlers[fd];
          epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
          close(fd);
          log << Logger::info << "client disconnected: " << fd << Logger::endl;
        }
      }
    }
  }
  close(s);
}
