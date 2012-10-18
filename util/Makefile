# Joshua Elliott
# Makefile for config static library

OBJS= config/Config.o config/Tokenizer.o http/HTTPRequest.o http/HTTPResponse.o http/Tokenizer.o http/URL.o
LIBS= libconfig.a libhttp.a

.PHONY: all clean realclean

all : $(LIBS)

# config
libconfig.a : config/Config.o config/Tokenizer.o
	ar r libconfig.a config/Config.o config/Tokenizer.o

config/Config.o : config/Config.cc config/Config.h
	g++ -o config/Config.o -c -I config config/Config.cc

config/Tokenizer.o : config/Tokenizer.cc config/Tokenizer.h
	g++ -o config/Tokenizer.o -c -I config config/Tokenizer.cc

# http
libhttp.a : http/HTTPRequest.o http/HTTPResponse.o http/Tokenizer.o http/URL.o
	ar r libhttp.a http/HTTPRequest.o http/HTTPResponse.o http/Tokenizer.o http/URL.o

http/HTTPRequest.o : http/HTTPRequest.cc http/HTTPRequest.h
	g++ -o http/HTTPRequest.o -c -I http http/HTTPRequest.cc

http/HTTPResponse.o : http/HTTPResponse.cc http/HTTPResponse.h
	g++ -o http/HTTPResponse.o -c -I http http/HTTPResponse.cc

http/URL.o : http/URL.cc http/URL.h
	g++ -o http/URL.o -c -I http http/URL.cc

http/Tokenizer.o : http/Tokenizer.cc http/Tokenizer.h
	g++ -o http/Tokenizer.o -c -I http http/Tokenizer.cc

clean:
	rm -f $(OBJS)

realclean:
	rm -f $(OBJS) $(LIBS)
