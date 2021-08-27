# $Id: Makefile,v 1.2 2001/05/31 21:32:21 marvin Exp $
#
# NICs (3c509, 3c905)
#
MODULEDIR=$(ROOTMODULEDIR)/netdev

MODULES=dummy.o

ifdef DRIVER_NETDEV_3C5x9
MODULES += 3c5x9.o
endif

ifdef DRIVER_NETDEV_NE2k
MODULES += ne.o
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(MODULES)
	mkdir -p $(MODULEDIR)
	for i in $(MODULES); do mv $$i $(MODULEDIR); done
	rm -f $(MODULEDIR)/dummy.o;

clean:
	rm -f $(MODULES)

dummy.o:
	touch dummy.o

include $(SRCDIR)/makefile.inc
