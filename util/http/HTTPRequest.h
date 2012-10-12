// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HTTPRequest_h
#define HTTPRequest_h

#include <map>
#include <string>
#include <vector>

#include "Tokenizer.h"

//
// HTTPRequest
//
// This class helps the caller to parse an incoming HTTPRequest or to
// create an outgoing HTTPRequest.
//
// When calling the parse() method, you must pass in a string that
// contains all HTTP headers.  Then call the method(), uri(),
// version(), and header() methods to access the results.
//
class HTTPRequest {
 public:
      // Constructor and deconstructor
    HTTPRequest();
    ~HTTPRequest();

      // Parse an incoming HTTPRequest.  Pass in a string that
      // contains all the HTTP headers, including appropriate newlines
      // at the end of each line.
    void parse(std::string&);

      // Get/set the method, URI, and version fields of the request line
    std::string& method();
    void method(std::string);
    void method(const char*);
    std::string& uri();
    void uri(std::string);
    void uri(const char*);
    std::string& version();
    void version(std::string);
    void version(const char*);

      // Get/set the request headers
    std::string& header(std::string);
    void header(std::string,std::string);
    void header(std::string,int);
    void header(const char*,const char*);
    void header(const char*,int);

      // Return the response as a string
    std::string str();
      // Return a partial response as a string -- leave out the final
      // CRLF so that the caller can add more headers
    std::string pstr();
      // Print the response
    void print();
    
 private:
      // Get the request fields
    void getRequest(std::string&);
      // Get a header and its corresponding value
    void getHeader(std::string&);

    std::string method_;
    std::string uri_;
    std::string version_;

    std::map<std::string,std::string> headers_;

    Tokenizer tokenizer_;
};

#endif
