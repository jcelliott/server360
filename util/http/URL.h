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

#ifndef URL_h
#define URL_h

#include <stdlib.h>
#include <string>

#include "Tokenizer.h"

using namespace std;

//
// URL
//
// URL parsing utility
//

class URL {
 public:
      // Constructor and deconstructor
    URL();
    ~URL();

    string url();
    string host();
    int port();
    string path();
    string file();

    void parse(string& url);

private:
    string url_;
    string host_;
    int port_;
    string path_;
    string file_;

    Tokenizer tokenizer_;
};

#endif
