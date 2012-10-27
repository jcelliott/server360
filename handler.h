// C includes
#include <errno.h>
// #include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
// #include <sys/types.h>

#include <iostream>
#include <sstream>

#include "util/http/HTTPRequest.h"
#include "util/http/HTTPResponse.h"
#include "util/http/URL.h"
#include "util/config/Config.h"
#include "util/logger.h"

#define BUFSIZE 2048

using namespace std;

class Handler {
public:
  Handler(int _client, Config & _config, Logger & log);
  ~Handler();

  // Handle a client request, returns true if the server should keep handling
  // requests for this client, otherwise returns false.
  bool handle();

private:
  // Reads once from the socket. Returns false if an error occurred.
  bool readRequest();

  // Gets a single request from the read buffer
  bool getRequest();

  // Constructs the response object (read from files, etc.). Returns false if an error occurred.
  void createResponse();

  // Sends the headers and if possible a file to the client
  bool sendResponse();

  // try to get a file descriptor
  void getFile(string path);

  // Get the date and time for the header
  string date ( time_t t );

  // Clean up and reset handler state
  void cleanup();

  char buf_[1600];
  int client, resFile;
  Config & config;
  bool debug;
  string read;
  HTTPRequest* req;
  HTTPResponse* res;

  Logger & log;
};

