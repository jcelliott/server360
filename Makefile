# Joshua Elliott
# Makefile for CS360 Lab 4: Web Server

CXX=	g++ -g
SERVEROBJS=	web.o server.o handler.o
OBJS =	$(SERVEROBJS)

SLIBS= util/libconfig.a
LIBS= -lrt -O2

# CCFLAGS= -g
CCFLAGS=

.PHONY: all clean realclean

all: server

server: $(SERVEROBJS) $(SLIBS)
	$(CXX) -o web $(SERVEROBJS) $(SLIBS) $(LIBS)

util/libconfig.a :
	@ make -C util libconfig.a

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)
	@ make -C util clean

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) web
	@ make -C util realclean


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	@$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

-include	$(OBJS:.o=.d)
