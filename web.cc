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

  Logger & log = Logger::GetLog();
  log.Mute();

  // process command line options using getopt()
  // see "man 3 getopt"
  int option;
  while ((option = getopt(argc,argv,"p:dv")) != -1) {
    switch (option) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'd':
        log.Unmute();
        log.SetMinPriority(Logger::INFO);
        break;
      case 'v':
        log.Unmute();
        log.SetMinPriority(Logger::DEBUG);
        break;
      default:
        cout << "server [-p port] [-d]" << endl
             << "-d show informational debugging output" << endl
             << "-v show verbose debugging output" << endl;
        exit(EXIT_FAILURE);
    }
  }

  log << Logger::info << "Log output ON\n";
  log << Logger::debug << "Verbose debugging output ON\n";
  log << Logger::info << "Server starting...\n";

  // get config parameters
  Config config;
  config.parse("web.conf");

  // start server
  Server s(port, config, log);
  s.start();

}

