# Joshua Elliott
# Makefile for config static library

OBJS= config/Config.o config/Tokenizer.o

.PHONY: clean realclean

all : libconfig.a

libconfig.a : config/Config.o config/Tokenizer.o
	ar r libconfig.a config/Config.o config/Tokenizer.o

config/Config.o : config/Config.cc config/Config.h
	g++ -o config/Config.o -c -I config config/Config.cc

config/Tokenizer.o : config/Tokenizer.cc config/Tokenizer.h
	g++ -o config/Tokenizer.o -c -I config config/Tokenizer.cc

clean:
	rm -f $(OBJS)

realclean:
	rm -f $(OBJS) libconfig.a
