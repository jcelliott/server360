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

#include <iostream>
#include <sstream>

#include "HTTPRequest.h"

using namespace std;

//
// HTTPRequest: public methods
//

HTTPRequest::HTTPRequest()
{
    method_ = string("GET");
    uri_ = string("/");
    version_ = string("HTTP/1.1");
}

HTTPRequest::~HTTPRequest()
{
}

void
HTTPRequest::parse(string& http)
{
      // empty everything
    method_.clear();
    uri_.clear();
    version_.clear();
    headers_.clear();

      // check for "\r\n\r\n"
    string::size_type pos = http.find("\r\n\r\n",0);
    if (pos == string::npos)
        return;
      // copy up to pos
    string httpHeaders = string(http,0,pos);

      // tokenize the string
    vector<string> tokens = tokenizer_.parse(httpHeaders, "\r\n");
    vector<string>::iterator line = tokens.begin();
    if (tokens.begin() == tokens.end())
        return;

      // parse request line
    getRequest(*line);
    line++;

      // parse headers
    for (line; line != tokens.end(); line++)
        getHeader(*line);
}

string&
HTTPRequest::method()
{
    return method_;
}

void
HTTPRequest::method(std::string m)
{
    method_ = m;
}

void
HTTPRequest::method(const char* m)
{
    method_ = string(m);
}


string&
HTTPRequest::uri()
{
    return uri_;
}

void
HTTPRequest::uri(std::string u)
{
    uri_ = u;
}

void
HTTPRequest::uri(const char* u)
{
    uri_ = string(u);
}


string&
HTTPRequest::version()
{
    return version_;
}

void
HTTPRequest::version(std::string v)
{
    version_ = v;
}

void
HTTPRequest::version(const char* v)
{
    version_ = string(v);
}

string&
HTTPRequest::header(string field)
{
    return headers_[field];
}

void
HTTPRequest::header(std::string field,std::string value)
{
    headers_[field] = value;
}

void
HTTPRequest::header(string field,int value)
{
    stringstream s;
    s << value;
    headers_[field] = s.str();
}

void
HTTPRequest::header(const char* field,const char* value)
{
    headers_[string(field)] = string(value);
}

void
HTTPRequest::header(const char* field,int value)
{
    stringstream s;
    s << value;
    headers_[string(field)] = s.str();
}

string
HTTPRequest::str()
{
    string message;
      // get the request line
    message = method_ + " " + uri_ + " " + version_ + "\r\n";

      // get the headers
    map<string,string>::iterator mk;
    for (mk = headers_.begin(); mk != headers_.end(); mk++)
        message += mk->first + ": " + mk->second + "\r\n";
    message += "\r\n";
    return message;
}

string
HTTPRequest::pstr()
{
    string message;
      // get the request line
    message = method_ + " " + uri_ + " " + version_ + "\r\n";

      // get the headers
    map<string,string>::iterator mk;
    for (mk = headers_.begin(); mk != headers_.end(); mk++)
        message += mk->first + ": " + mk->second + "\r\n";
    return message;
}

void
HTTPRequest::print()
{
    string message = str();
    cout << message;
}

//
// HTTPRequest: private methods
//

void
HTTPRequest::getRequest(string &line)
{
      // parse line into an HTTP request
    vector<string>tokens = tokenizer_.parse(line);
    if (tokens.size() < 3)
        return;
    method_ = tokens.at(0);
    uri_ = tokens.at(1);
    version_ = tokens.at(2);
}

void
HTTPRequest::getHeader(string &line)
{
      // parse line into an HTTP header
    vector<string> tokens = tokenizer_.parseTwo(line, ":");
    if (tokens.size() < 2)
        return;
    string name = tokens.at(0);
    string value = tokens.at(1);
    try {
        while (value.at(0) == ' ')
            value.erase(0,1);
    }
    catch (...) {
        return;
    }
    headers_[name] = value;
}
