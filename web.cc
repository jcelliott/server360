/**
 * Joshua Elliott
 * web.h
 * CS360 - Lab 4 - Web Server
 * Processes command line args and config parameters. Starts server.
 */

#include "server.h"
#include "util/config/Config.h"

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

  if(debug) {
    cout << "Debugging output ON" << endl;
    cout << "Server starting... " << endl;
  }

  // get config parameters
  Config config;
  config.parse("web.conf");

  // start server
  Server s(port, config, debug);
  s.start();

}
