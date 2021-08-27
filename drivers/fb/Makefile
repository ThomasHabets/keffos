# $Id: Makefile,v 1.2 2001/05/31 21:32:20 marvin Exp $
#
# Framebuffer devices (graphic adapters)
#
MODULEDIR=$(ROOTMODULEDIR)/fb

MODULES=dummy.o

ifdef DRIVER_FB_VESA1
MODULES += vesa1.o
endif

ifdef DRIVER_FB_VESA2
MODULES += vesa2.o
endif

ifdef DRIVER_FB_VESA3
MODULES += vesa3.o
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
