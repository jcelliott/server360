# Joshua Elliott
# Makefile for CS360 Lab 4: Web Server

CXX=	g++
SERVEROBJS=	web.o server.o handler.o
OBJS =	$(SERVEROBJS)

SLIBS= util/libconfig.a util/libhttp.a util/tri_logger.o
LIBS= -lrt -O2

CXXFLAGS= -g
# CCFLAGS=

.PHONY: all clean realclean

all: server

server: $(SERVEROBJS) $(SLIBS)
	$(CXX) -o web $(SERVEROBJS) $(SLIBS) $(LIBS)

$(SLIBS) :
	@ make -C util all

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
