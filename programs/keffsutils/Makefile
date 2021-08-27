# $Id: Makefile,v 1.2 2001/05/31 21:32:23 marvin Exp $
# KeffOS
#
# Utilities for managing keffs filesystems
#

all: mkkeffs dumpkeffs

mkkeffs: mkkeffs.c
	gcc -g -I$(SRCDIR)/fs -I$(SRCDIR)/kernel/include -o $@ $<

dumpkeffs: dumpkeffs.c
	gcc -g -I$(SRCDIR)/fs -I$(SRCDIR)/kernel/include -o $@ $<

clean:
	rm -f mkkeffs dumpkeffs
