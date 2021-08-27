# $Id: Makefile,v 1.2 2001/05/31 21:32:20 marvin Exp $
#
# Character drives (parport, serial, etc.)
#
MODULEDIR=$(ROOTMODULEDIR)/char

MODULES=dummy.o

ifdef DRIVER_CHAR_PARPORT
MODULES += parport.o
endif

ifdef DRIVER_CHAR_SERIAL
MODULES += serial.o
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
