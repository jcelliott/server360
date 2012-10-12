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

#ifndef HTTPResponse_h
#define HTTPResponse_h

#include <map>
#include <string>
#include <vector>

#include "Tokenizer.h"

//
// HTTPResponse
//
// This class helps the caller to parse an incoming HTTPResponse or to
// create an outgoing HTTPResponse.
//
// When calling the parse() method, you must pass in a string that
// contains all HTTP headers.  Then call the method(), uri(),
// version(), and header() methods to access the results.
//
class HTTPResponse {
 public:
      // Constructor and deconstructor
    HTTPResponse();
    ~HTTPResponse();

      // Parse an incoming HTTPResponse.  Pass in a string that
      // contains all the HTTP headers, including appropriate newlines
      // at the end of each line.
    void parse(std::string&);

      // Get/set the method, URI, and version fields of the response line
    std::string version();
    void version(std::string);
    void version(const char*);
    std::string code();
    void code(std::string);
    void code(const char*);
    std::string phrase();
    void phrase(std::string);
    void phrase(const char*);

      // Get/set the response headers
    std::string header(std::string);
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
      // Get the response fields
    void getResponse(std::string&);
      // Get a header and its corresponding value
    void getHeader(std::string&);

    std::string version_;
    std::string code_;
    std::string phrase_;

    std::map<std::string,std::string> headers_;

    Tokenizer tokenizer_;
};

#endif
