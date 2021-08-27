# $Id: Makefile,v 1.2 2001/05/31 21:32:23 marvin Exp $
#
TARGETS=config

CC=gcc
CCFLAGS=-g -O

#
# globals
#
all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o config2

#
# Generic rules
#
%.o: %.c
	$(CC) $(CCFLAGS) -c -o $@ $<

#
# Specific rules
#
config: config.o
	$(CC) $(CCFLAGS) -o $@ $<
