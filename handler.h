// C includes
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#include "util/http/HTTPRequest.h"

#define BUFSIZE 1024

using namespace std;

class Handler {
public:
  Handler();
  ~Handler();

  // Handle a client request, returns true if the server should keep handling
  // requests for this client, otherwise returns false.
  bool handle(int client);

  HTTPRequest* readHead();

private:
  char buf_[1600];
  string leftover;
  int port, sock;
  bool debug;
};

