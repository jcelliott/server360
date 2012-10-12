# Makefile for CS 360 example code

CXX= g++ -g
TOBJS= test.o Config.o Tokenizer.o
OBJS = $(TOBJS)

LIBS=

CCFLAGS= -g

all: 	test

test:   $(TOBJS)
	$(CXX) -o test $(TOBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) test


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
