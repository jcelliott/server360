#include <iostream>
#include <string>

#include "Config.h"

using namespace std;

int main()
{
      // parse Configuration
    Config cfg;
    cfg.parse("web.conf");

    cout << "Root for carmelo: " << cfg.host("carmelo.cs.byu.edu") << endl;
    cout << "Root for localhost: " << cfg.host("localhost") << endl;

    cout << "Media for txt: " << cfg.media("txt") << endl;
    cout << "Media for html: " << cfg.media("html") << endl;
    cout << "Media for jpg: " << cfg.media("jpg") << endl;
    cout << "Media for gif: " << cfg.media("gif") << endl;
    cout << "Media for png: " << cfg.media("png") << endl;
    cout << "Media for pdf: " << cfg.media("pdf") << endl;
    cout << "Media for py: " << cfg.media("py") << endl;

    cout << "Timeout parameter in seconds: " << cfg.parameter("timeout") << endl;
}
