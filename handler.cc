#include "handler.h"

using namespace std;

Handler::Handler() {}

Handler::~Handler() {}

HTTPRequest* Handler::readHead() {

  string read = "";
  leftover = "";
  char *buf = (char *) calloc(BUFSIZE,sizeof(char));
  int endOfHead = 0;

  while ((endOfHead = read.find("\r\n\r\n")) == string::npos) {
    memset(buf,0,BUFSIZE);
    int nread = recv(sock,buf,BUFSIZE,0);
    if (nread < 0) {
      if (errno == EINTR)
        // the socket call was interrupted -- try again
        continue;
      else
        // an error occurred, so break out
        if(debug) cout << "An error occurred in readHead()" << endl;
      break;
    } else if (nread == 0) {
      // the socket is closed
      if(debug) cout << "The socket is closed in readHead()" << endl;
      break;
    }
    read.append(buf, nread);

    // if(debug) cout << "received: " << buf;
  }
  if(read.length() > endOfHead + 4) {
    leftover = read.substr(endOfHead + 4);
  }

  // TODO: check this, it was just copypastad
  HTTPRequest* req = new HTTPRequest();
  req->parse(read);
  if(debug) cout << "Response:\n" << req->str() << endl;
  return req;
}

bool Handler::handle(int client) {
  char *ptr;

  // read a request
  memset(buf_,0,1501);
  int nread = recv(client,buf_,1500,0);
  if (nread < 0) {
    if (errno == EINTR)
      // the socket call was interrupted -- try again
      return true;
    // an error occurred so stop processing
    perror("recv");
    return false;
  } else if (nread == 0) {
    // the socket is closed
    return false;
  }

  // write the same data back
  ptr = buf_;
  int nwritten;
  int nleft = nread;
  while (nleft) {
    if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
      if (errno == EINTR) {
        // the socket call was interrupted -- try again
        continue;
      } else {
        // an error occurred, so break out
        perror("write");
        return false;
      }
    } else if (nwritten == 0) {
      // the socket is closed
      return false;
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return true;
}
