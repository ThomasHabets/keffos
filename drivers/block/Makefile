# $Id: Makefile,v 1.2 2001/05/31 21:32:20 marvin Exp $
#
# Modules for block devices (ide, floppy, etc.)
#
MODULEDIR=$(ROOTMODULEDIR)/block

MODULES=dummy.o

ifdef DRIVER_BLOCK_IDE
MODULES += ide.o
endif

ifdef DRIVER_BLOCK_FLOPPY
MODULES += floppy.o
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
