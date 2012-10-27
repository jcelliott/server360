#include "handler.h"

using namespace std;

Handler::Handler(int _client, Config & _config, Logger & _log) 
    : client(_client), config(_config), log(_log) {
  log << Logger::info << "Created new handler for client: " << client << Logger::endl;
  resFile = 0;
}

Handler::~Handler() {}

bool Handler::handle() {
  if (!readRequest())
    return false;

  // handle as many requests as we got, because we won't call handle() again
  // until there is data to be read from the socket
  while(getRequest()) {
    createResponse();
    if (!sendResponse())
      return false;
    cleanup();
  }
  return true;
}

// Clean up and reset handler state
void Handler::cleanup() {
  log << Logger::debug << "cleanup()\n";
  if (req != NULL)
    delete req;
  if (res != NULL)
    delete res;
  resFile = 0;
}

// Sends the headers and if possible a file to the client
bool Handler::sendResponse() {
  log << Logger::debug << "sendResponse()\n";
  // convert response to string
  string resStr = res->str();

  // send headers
  char buf[resStr.length()];
  strcpy(buf, resStr.c_str());
  char* ptr = buf;
  int nwritten;
  int nleft = resStr.length();

  while (nleft) {
    if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
      if (errno == EINTR) {
        // the socket call was interrupted -- try again
        return true;
      } else {
        // an error occurred, so break out
        log << Logger::error << "An error occurred in writeResponse\n";
        return false;
      }
    } else if (nwritten == 0) {
      // the socket is closed
      // if(debug) cout << "The socket is closed in writeResponse" << endl;
      return false;
    }
    nleft -= nwritten;
    ptr += nwritten;
  }

  // send a file if we have one
  if (resFile != 0) {
    size_t count = atoi(res->header("Content-Size").c_str());
    while (count > 0) {
      int sent = sendfile(resFile, client, NULL, count);
      if (sent < 0) {
        log << Logger::error << "Error sending file\n";
        perror("sendfile");
        return false;
      }
      count -= sent;
    }
  }

}

// Gets a single request from the read buffer
bool Handler::getRequest() {

  size_t endOfReq = read.find("\r\n\r\n");
  if (endOfReq == string::npos) {
    log << Logger::debug << "Didn't read enough to get a full request\n";
    return false;
  }

  string reqStr = read.substr(0, endOfReq + 3);
  read = read.substr(endOfReq + 4);
  req = new HTTPRequest();
  req->parse(reqStr);
  log << Logger::info << "Request:\n" << req->str() << Logger::endl;
  return true;
}

// Reads once from the socket. Returns false if an error occurred.
bool Handler::readRequest() {
  log << Logger::debug << "readRequest\n";

  char *buf = (char *) calloc(BUFSIZE,sizeof(char));
  memset(buf,0,BUFSIZE);

  int nread = recv(client,buf,BUFSIZE,0);
  if (nread < 0) {
    if (errno == EINTR)
      // the socket call was interrupted -- try again
      return true;
    else
      // an error occurred, so break out
      log << Logger::error << "An error occurred in readHead()\n";
      return false;
  } else if (nread == 0) {
    // the socket is closed
    log << Logger::error << "The socket is closed in readHead()\n";
    return false;
  }
  read.append(buf, nread);
  return true;
}

// Constructs the response object (read from files, etc.).
void Handler::createResponse() {
  log << Logger::debug << "createResponse()\n";
  res = new HTTPResponse();
  res->version("HTTP/1.1");
  res->header("Date", date(time(NULL)));
  res->header("Server", "Server360");
  // add Server header

  // Check for empty method or URI (400)
  if (req->method() == "" || req->uri() == "") {
    log << Logger::warning << "empty method or URI\n";
    res->code("400");
    res->phrase("Bad Request"); 
    return;
  }

  // Check method is implemented (501)
  if (req->method() != "GET") {
    log << Logger::warning << "method is not implemented\n";
    res->code("501");
    res->phrase("Not Implemented");
    return;
  }

  // Check if host is handled by server (400)
  if (config.host(req->header("Host")).empty()) {
    log << Logger::warning << "host is not handled by this server\n";
    res->code("400");
    res->phrase("Bad Request");
    return;
  }

  // Parse document path from host root and path
  string path = config.host(req->header("Host"));
  URL url;
  url.parse(req->uri());
  if (url.path().compare("/") == 0) {
    path.append("/index.html");
  } else {
    path.append(url.path());
  }
  log << Logger::info << "parsed path: " << path << Logger::endl;

  getFile(path);

  return;
}

// try to get a file descriptor
void Handler::getFile(string path) {
  log << Logger::debug << "getFile()\n";
  // Get file (check for permission (400), not found (404), error (500))
  int file = open(path.c_str(), 0);
  if (file == -1) {
    // There was an error
    if (errno == EACCES) {
      // 403 Forbidden
      log << Logger::warning << "403 Forbidden\n";
      res->code("403");
      res->phrase("Bad Request");
      return;
    } else if (errno == ENOENT) {
      log << Logger::warning << "404 Not Found\n";
      res->code("404");
      res->phrase("Not Found");
      file = open(config.host(req->header("Host")).append("404.html").c_str(), 0);
      if (file == -1)
        return;
    } else {
      log << Logger::error << "500 Internal Server Error\n";
      res->code("500");
      res->phrase("Internal Server Error");
      return;
    }
  }

  // Content-Length, Last-Modified headers
  log << Logger::debug << "get content-length and last-modified headers\n";
  struct stat st;
  if (fstat(file, &st) != 0) {
    res->code("500");
    res->phrase("Internal Server Error");
    return;
  }
  res->header("Last-Modified", date(st.st_mtime));
  stringstream s;
  s << st.st_size;
  res->header("Content-Size", s.str());

  // We've validated everything, now we know we can send the file
  resFile = file;

  // Content-Type header
  log << Logger::debug << "get content-type header\n";
  size_t extloc = path.find_last_of('.');
  if (extloc == string::npos) {
    res->header("Content-Type", "text/plain");
  } else {
    string ext = path.substr(extloc);
    string type = config.media(ext);
    if (type.empty()) {
      res->header("Content-Type", "text/plain");
    } else {
      res->header("Content-Type", type);
    }
  }

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

