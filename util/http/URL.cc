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

#include "URL.h"

//
// Tokenizer: public methods
//

URL::URL()
{
    port_ = 80;
}

URL::~URL()
{
}

string
URL::url()
{
  return url_;
}

string
URL::host()
{
  return host_;
}

int
URL::port()
{
  return port_;
}

string
URL::path()
{
  return path_;
}

string
URL::file()
{
  return file_;
}

void
URL::parse(string& url)
{
    // strip "http://" if present
    string::size_type pos = url.find("http://");
    string hostpath;
    if (pos == 0)
        hostpath = url.substr(7,string::npos);
    else
        hostpath = url;

    // get host and port
    string hostport;
    pos = hostpath.find_first_of("/", 0);
    if (pos == string::npos) {
      hostport = hostpath;
      path_ = "/";
      file_ = "index.html";
    } else {
      hostport = hostpath.substr(0,pos);
      path_ = hostpath.substr(pos,string::npos);
      if (*(path_.rbegin()) == '/') {
          file_ = "index.html";
      } else {
          vector<string> parts = tokenizer_.parse(path_,"/");
          file_ = parts.back();
      }
    }

    // separate host and port
    vector<string> tokens = tokenizer_.parseTwo(hostport, ":");
    host_ = tokens.at(0);
    if (tokens.at(1) == "")
        port_ = 80;
    else
        port_ = atoi(tokens.at(1).c_str());
}
