#ifndef Config_h
#define Config_h

#include <map>
#include <string>

#include "Tokenizer.h"

// Config

// Parse a configuration file for the web server

class Config {
 public:
    Config();
    ~Config();

      // Parse a configuration file
    void parse(const char*);

      // Get a configuration setting
    std::string& host(std::string);
    std::string& media(std::string);
    std::string& parameter(std::string);

 private:
      // Parse a line of the configuration file
    void parseLine(std::string&);

    std::map<std::string,std::string> host_;
    std::map<std::string,std::string> media_;
    std::map<std::string,std::string> parameter_;

    Tokenizer tokenizer_;
};

#endif
