/**
 * Joshua Elliott
 * fileRunner.h
 * CS360 - Lab 4 - Web Server
 * 
 */

#ifndef FILE_RUNNER_H
#define FILE_RUNNER_H

// C includes
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

// C++ includes
#include <iostream>
#include <string>

// local includes
#include "handler.h"

#define MAX_EVENTS 1000

using namespace std;


class FileRunner {
public:
  FileRunner();
  ~FileRunner();


private:

};

#endif

