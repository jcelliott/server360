/**
 * Joshua Elliott
 * web.h
 * CS360 - Lab 4 - Web Server
 * Processes command line args and config parameters. Starts server.
 */

#include "server.h"
#include "util/config/Config.h"
#include "util/logger.h"

using namespace std;

int main(int argc, char **argv) {
  
  // setup default arguments
  int port = 8080;
  bool debug = false;

  int option;
  // process command line options using getopt()
  // see "man 3 getopt"
  while ((option = getopt(argc,argv,"p:d")) != -1) {
    switch (option) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'd':
        debug = true;
        break;
      default:
        cout << "server [-p port] [-d]" << endl;
        exit(EXIT_FAILURE);
    }
  }

  Logger & log = Logger::GetLog();
  if(debug) {
    log.SetMinPriority(Logger::INFO);
    log << Logger::info << "Log output ON\n";
    log << Logger::info << "Server starting...\n";
  } else {
    log.Mute();
  }

  // get config parameters
  Config config;
  config.parse("web.conf");

  // start server
  Server s(port, config, log);
  s.start();

}

