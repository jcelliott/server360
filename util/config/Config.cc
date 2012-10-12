#include <fstream>
#include <iostream>

#include "Config.h"

using namespace std;

//
// Config: public methods
//

Config::Config()
{
}

Config::~Config()
{
}

void Config::parse(const char* filename)
{
    host_.clear();
    media_.clear();
    parameter_.clear();

    string line;

      // open file
    ifstream file(filename);
    if (!file.is_open())
        return;

      // parse file
    while (!file.eof()) {
        getline(file,line);
        parseLine(line);
    }

      // close file
    file.close();
}

string&
Config::host(string name)
{
    return host_[name];
}

string&
Config::media(string name)
{
    return media_[name];
}

string&
Config::parameter(string name)
{
    return parameter_[name];
}

//
// Config: private methods
//

void Config::parseLine(std::string &line)
{
    vector<string> tokens = tokenizer_.parse(line);
    if (tokens.size() < 3)
        return;
    if (tokens.at(0) == "host")
        host_[tokens.at(1)] = tokens.at(2);
    else if (tokens.at(0) == "media")
        media_[tokens.at(1)] = tokens.at(2);
    else if (tokens.at(0) == "parameter")
        parameter_[tokens.at(1)] = tokens.at(2);
}
