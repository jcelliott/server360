#ifndef Tokenizer_h
#define Tokenizer_h

#include <string>
#include <vector>

//
// Tokenizer
//
// Tokenizes a string
//

class Tokenizer {
 public:
      // Constructor and deconstructor
    Tokenizer();
    ~Tokenizer();

      // Parse a string into a vector of tokens separated by spaces (by default)
      // or any other delimiters
    std::vector<std::string>& parse(std::string&, const std::string& = " ");
      // Parse a string into a vector of at most two tokens
    std::vector<std::string>& parseTwo(std::string&, const std::string& = " ");

 private:
    std::vector<std::string> tokens_;
};

#endif
