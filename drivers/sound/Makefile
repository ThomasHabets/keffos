# $Id: Makefile,v 1.2 2001/05/31 21:32:21 marvin Exp $
#
# Soundcards
#
MODULEDIR=$(ROOTMODULEDIR)/sound

MODULES=dummy.o

ifdef DRIVER_SOUND_SB
MODULES += sb.o
endif

ifdef DRIVER_SOUND_GUS
MODULES += gus.o
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
