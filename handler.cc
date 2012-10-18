#include "handler.h"

Handler::Handler() {}
Handler::~Handler() {}

bool Handler::handle(int client) {
    char *ptr;

    // read a request
    memset(buf_,0,1501);
    int nread = recv(client,buf_,1500,0);
    if (nread < 0) {
        if (errno == EINTR)
            // the socket call was interrupted -- try again
            return true;
        // an error occurred so stop processing
        perror("recv");
        return false;
    } else if (nread == 0) {
        // the socket is closed
        return false;
    }

    // write the same data back
    ptr = buf_;
    int nwritten;
    int nleft = nread;
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}
