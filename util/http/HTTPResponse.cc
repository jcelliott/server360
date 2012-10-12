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

#include "HTTPResponse.h"

using namespace std;

//
// HTTPResponse: public methods
//

HTTPResponse::HTTPResponse()
{
    version_ = string("HTTP/1.1");
    code_ = string("200");
    phrase_ = string("OK");
}

HTTPResponse::~HTTPResponse()
{
}

void
HTTPResponse::parse(string& http)
{
      // empty everything
    version_.clear();
    code_.clear();
    phrase_.clear();
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

      // parse response line
    getResponse(*line);
    line++;

      // parse headers
    for (line; line != tokens.end(); line++)
        getHeader(*line);
}

string
HTTPResponse::version()
{
    return version_;
}

void
HTTPResponse::version(string v)
{
    version_ = v;
}

void
HTTPResponse::version(const char* v)
{
    version_ = string(v);
}


string
HTTPResponse::code()
{
    return code_;
}

void
HTTPResponse::code(string c)
{
    code_ = c;
}

void
HTTPResponse::code(const char* c)
{
    code_ = string(c);
}

string
HTTPResponse::phrase()
{
    return phrase_;
}

void
HTTPResponse::phrase(string p)
{
    phrase_ = p;
}

void
HTTPResponse::phrase(const char* p)
{
    phrase_ = string(p);
}

string
HTTPResponse::header(string field)
{
    return headers_[field];
}

void
HTTPResponse::header(string field,string value)
{
    headers_[field] = value;
}

void
HTTPResponse::header(string field,int value)
{
    stringstream s;
    s << value;
    headers_[field] = s.str();
}

void
HTTPResponse::header(const char* field,const char* value)
{
    headers_[string(field)] = string(value);
}

void
HTTPResponse::header(const char* field,int value)
{
    stringstream s;
    s << value;
    headers_[string(field)] = s.str();
}

string
HTTPResponse::str()
{
    string message;
      // get the response line
    message = version_ + " " + code_ + " " + phrase_ + "\r\n";

      // get the headers
    map<string,string>::iterator mk;
    for (mk = headers_.begin(); mk != headers_.end(); mk++)
        message += mk->first + ": " + mk->second + "\r\n";
    message += "\r\n";
    return message;
}

string
HTTPResponse::pstr()
{
    string message;
      // get the response line
    message = version_ + " " + code_ + " " + phrase_ + "\r\n";

      // get the headers
    map<string,string>::iterator mk;
    for (mk = headers_.begin(); mk != headers_.end(); mk++)
        message += mk->first + ": " + mk->second + "\r\n";
    return message;
}

void
HTTPResponse::print()
{
    string message = str();
    cout << message;
}

//
// HTTPResponse: private methods
//

void
HTTPResponse::getResponse(string &line)
{
      // parse line into an HTTP response
    vector<string>tokens = tokenizer_.parse(line);
    if (tokens.size() < 3)
        return;
    version_ = tokens.at(0);
    code_ = tokens.at(1);
    phrase_ = tokens.at(2);
    for (int i=3; i<tokens.size(); i++) {
        phrase_ += " ";
        phrase_ += tokens.at(i);
    }
}

void
HTTPResponse::getHeader(string &line)
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
