// C includes
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

using namespace std;

class Handler {
public:
    Handler();
    ~Handler();

    // Handle a client request, returns true if the server should keep handling
    // requests for this client, otherwise returns false.
    bool handle(int client);

private:
    char buf_[1600];
};

