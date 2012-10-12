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
    tokens_.push_back(str.substr(lastPos, string::npos));

    return tokens_;
}
