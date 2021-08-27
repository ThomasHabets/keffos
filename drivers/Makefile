# $Id: Makefile,v 1.2 2001/05/31 21:32:20 marvin Exp $
#
# Drivers for physical devices such as aic7xxx, ide, 3c509, etc.
#
DIRS := dummy

ifdef DRIVER_BLOCK
DIRS += block
endif

ifdef DRIVER_CHAR
DIRS += char
endif

ifdef DRIVER_NETDEV
DIRS += netdev
endif

ifdef DRIVER_SOUND
DIRS += sound
endif

ifdef DRIVER_FB
DIRS += fb
endif

all:
	@echo Making drivers
	for i in $(DIRS); do make -C $$i;done

clean:
	@echo Cleaning drivers
	for i in $(DIRS); do make clean -C $$i;done

realclean:
	@echo Realcleaning drivers
