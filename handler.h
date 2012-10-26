// C includes
#include <errno.h>
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#include "util/http/HTTPRequest.h"
#include "util/http/HTTPResponse.h"
#include "util/http/URL.h"
#include "util/config/Config.h"

#define BUFSIZE 1024

using namespace std;

class Handler {
public:
  Handler(int _client, Config & _config, bool _debug);
  ~Handler();

  // Handle a client request, returns true if the server should keep handling
  // requests for this client, otherwise returns false.
  bool handle();

private:
  // Reads once from the socket. Returns false if an error occurred.
  bool readRequest();

  // Constructs the response object (read from files, etc.). Returns false if an error occurred.
  void createResponse();

  string date ( time_t t );

  char buf_[1600];
  int client;
  Config & config;
  bool debug;
  string read;
  HTTPRequest* req;
  HTTPResponse* res;
};

