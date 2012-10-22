#include "handler.h"

using namespace std;

Handler::Handler(int _client, Config & _config, bool _debug) 
    : client(_client), config(_config), debug(_debug) {
  req = NULL;
  res = NULL;
}

Handler::~Handler() {}

bool Handler::handle() {
  if (req == NULL)
    return readRequest();

  if (res == NULL)
    return createResponse();
}

// Reads once from the socket. Returns false if an error occurred.
bool Handler::readRequest() {

  char *buf = (char *) calloc(BUFSIZE,sizeof(char));
  memset(buf,0,BUFSIZE);

  int nread = recv(client,buf,BUFSIZE,0);
  if (nread < 0) {
    if (errno == EINTR)
      // the socket call was interrupted -- try again
      return true;
    else
      // an error occurred, so break out
      if(debug) cout << "An error occurred in readHead()" << endl;
      return false;
  } else if (nread == 0) {
    // the socket is closed
    if(debug) cout << "The socket is closed in readHead()" << endl;
    return false;
  }
  read.append(buf, nread);

  if (read.find("\r\n\r\n") == string::npos) {
    return true;
  }

  req = new HTTPRequest();
  req->parse(read);
  if(debug) cout << "Request:\n" << req->str() << endl;
  return true;
}

// Constructs the response object (read from files, etc.).
bool Handler::createResponse() {
  res = new HTTPResponse();
  res->version("HTTP/1.1");
  res->header("Date", date(time(NULL)));
  // add Server header

  // Check for empty method or URI (400)
  if (req->method() == "" || req->uri() == "") {
    res->code("400");
    res->phrase("Bad Request"); 
  }

  // Check method is implemented (501)
  if (req->method() != "GET") {
    res->code("501");
    res->phrase("Not Implemented");
  }

  // Check if host is handled by server (400)
  if (config.host(req->header("Host")).empty()) {
    res->code("400");
    res->phrase("Bad Request");
  }

  // Parse document path from host root and path

  // Get file (check for permission (400), not found (404), error (500))

  // Content-Type, Content-Length, Last-Modified headers

  return true;
}

string Handler::date ( time_t t )
{
  struct tm * gmt ;
  char buf [200];
  memset ( buf ,0 ,200);
  gmt = gmtime (& t );
  if ( gmt == NULL )
    return " " ;
  if ( strftime ( buf , sizeof ( buf ) ,
        " %a , % d % b % Y % H :% M :% S GMT " , gmt ) == 0)
    return " " ;
  return string ( buf );
}
