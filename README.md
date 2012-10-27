Server360
=========

[![Build Status](https://travis-ci.org/joshuacelliott/server360.png)](https://travis-ci.org/joshuacelliott/server360)

An event-driven web server in C++ for my Internet Programming class

**web.cc**

Parse command line options, create a server object and start it

**server.cc**

Set up socket structure, set up epoll, accept clients and add them to epoll.
Timeouts are handled by storing a timestamp of the last io event in the handler
object. Every time epoll_wait returns, the server iterates over all the active 
clients and checks their timestamps. If the time that has elapsed is greater 
than the timeout, the client is added to a list to be removed.

**handler.cc**

The main logic for handler is contained in the handle function: read once, 
handle as many requests as we got from the read (with createResponse and 
sendResponse), and return so the server can handle another client. The socket
io is done in readRequest and sendResponse. File io is handled in 
createResponse and getFile. After headers are handled, files are sent with
sendfile(). All the caching for clients is done within the Handler class. The 
main cache for each client is the read string. This is where all data is 
stored between calls to readRequest and getRequest.

