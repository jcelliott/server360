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

#include "Tokenizer.h"

using namespace std;

//
// Tokenizer: public methods
//

Tokenizer::Tokenizer()
{
}

Tokenizer::~Tokenizer()
{
}

std::vector<std::string>&
Tokenizer::parse(string& str, const string& delimiters)
{
      // Clear tokens
    tokens_.clear();

      // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (pos != string::npos || lastPos != string::npos) {
          // Found a token, add it to the vector.
        tokens_.push_back(str.substr(lastPos, pos - lastPos));
          // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
          // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }

    return tokens_;
}

std::vector<std::string>&
Tokenizer::parseTwo(string& str, const string& delimiters)
{
      // Clear tokens
    tokens_.clear();

      // find delimiter and break string in half
    string::size_type pos = str.find_first_of(delimiters, 0);
    string::size_type lastPos = str.find_first_not_of(delimiters, pos);
    tokens_.push_back(str.substr(0, pos));
    if (lastPos != string::npos)
        tokens_.push_back(str.substr(lastPos, string::npos));
    else
        tokens_.push_back("");

    return tokens_;
}
