# $Id: Makefile,v 1.2 2001/05/31 21:32:23 marvin Exp $
#
# Soft network drivers (ipv4/ipv6/ipx)
#
MODULEDIR=$(ROOTMODULEDIR)/net

MODULES=dummy.o

ifdef DRIVER_NET_IPv4
MODULES += ipv4.o
endif

ifdef DRIVER_NET_IPv6
MODULES += ipv6.o
endif

ifdef DRIVER_NET_IPX
MODULES += ipx.o
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
