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
#include <string>

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "URL.h"

using namespace std;

int main()
{
      // test HTTP Request parsing
    cout << endl << "*** Test 1 ***" << endl;
    string s = string("GET / HTTP/1.1\r\nHost:www.cnn.com\r\nUser-Agent:downloader\r\nFrom: zappala\r\n\r\n");
    HTTPRequest request;
    request.parse(s);
    cout << request.method() << " " << request.uri() << " "  << request.version() << endl;
    s = request.header(string("Host"));
    cout << "Host: " << s << endl;
    s = request.header(string("User-Agent"));
    cout << "User-Agent: " << s << endl;
    s = request.header(string("From"));
    cout << "From: " << s << endl;
    s = request.header(string("Data"));
    if (s.empty())
        cout << "No Data header" << endl;
    else
        cout << "Data: " << s << endl;

      // test HTTP Request creation
    cout << endl <<  "*** Test 2 ***" << endl;
    request = HTTPRequest();
    request.method("GET");
    request.uri(string("./index.html"));
    request.version("HTTP/1.1");
    request.header("Host","ilab.cs.byu.edu");
    request.header("From","zappala");
    request.header("Cookie",10);
    cout << request.str();

      // test HTTP Response parsing
    cout << endl <<  "*** Test 3 ***" << endl;
    s = string("HTTP/1.1 200 OK\r\nServer:Apache\r\nContent-Length:1000\r\n\r\n");
    HTTPResponse response;
    response.parse(s);
    cout << response.version() << " " << response.code() << " " << response.phrase() << endl;
    s = response.header(string("Server"));
    cout << "Server: " << s << endl;
    s = response.header(string("Content-Length"));
    cout << "Content-Length: " << s << endl;
    s = response.header(string("Data"));
    if (s.empty())
        cout << "No Data header" << endl;
    else
        cout << "Data: " << s << endl;

      // test HTTP Response creation
    cout << endl <<  "*** Test 4 ***" << endl;
    response = HTTPResponse();
    response.version("HTTP/1.1");
    response.code("200");
    response.phrase(string("OK"));
    response.header("Server","Apache");
    response.header("Content-Length",1000);
    cout << response.str();

    cout << endl <<  "*** Test 5 ***" << endl;
    URL u;
    string http = "http://www.google.com:8080/search.php";
    u.parse(http);
    cout << u.host() << " " << u.port() << " " << u.path() << " " << u.file() << endl;
    http = "www.google.com";
    u.parse(http);
    cout << u.host() << " " << u.port() << " " << u.path() << " " << u.file() << endl;
}
