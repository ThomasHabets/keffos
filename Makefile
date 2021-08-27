# $Id: Makefile,v 1.2 2001/05/31 21:32:20 marvin Exp $
#
# This is the main makefile for keffos
#
VERSION=0
RELEASE=0
PATCHLEVEL=3

SRCDIR=$(shell pwd)
ARCH=$(shell grep `uname -m` architectures | sed "s/`uname -m` //")

CDIRS=drivers fs net    # Dirs that don't take care of their own configging
NCDIRS=kernel programs  # dirs that do
DIRS=$(NCDIRS) $(CDIRS) # all dirs
DIRS=kernel

CONFIGPROG=tools/config
CFGFLS=$(shell find $(SRCDIR) -name "config.h.in" \
 | sed 's/config.h.in/config.h/g')

DRIVERLIST_NAME=DRIVERS
ROOTMODULEDIR=$(SRCDIR)/modules

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#
#   No user serviceable parts below
#
DEFINES=\
 -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DPATCHLEVEL=$(PATCHLEVEL)
INCLUDES=-I$(SRCDIR)/kernel/include
CFLAGS=$(INCLUDES) -Werror -pipe -Wall -w -fomit-frame-pointer $(DEFINES) -O6

.EXPORT_ALL_VARIABLES:

#---------#
# Targets
#---------#

# "Crash" if DRIVERLIST_NAME doesn't exist
ifeq ($(DRIVERLIST_NAME),$(wildcard $(DRIVERLIST_NAME)))
include $(DRIVERLIST_NAME)
all: kernel/Makefile $(CFGFLS)
	for i in $(DIRS); do make -C $$i; done
else
all:
	@echo "File \"$(DRIVERLIST_NAME)\" missing, restore it from backup"
endif

#----#
kernel/Makefile:
	ln -s $(SRCDIR)/kernel/arch/$(ARCH)/Makefile kernel/Makefile

$(CFGFLS): config

config: $(CONFIGPROG)
	echo $(CFGFLS)
	for i in $(CFGFLS); do \
		$(CONFIGPROG) $$i.in $$i $$i.help; \
	done
#----#
disk: all
	make disk -C kernel

floppy.img: kernel/floppy.img
	rm -f floppy.img
	ln -s kernel/floppy.img

kernel/floppy.img: all
	make floppy.img -C kernel

#----#
clean:
	for i in $(DIRS) tools; do make clean -C $$i;done
	rm -fr modules floppy.img

realclean: clean
	rm -f $(CFGFLS) kernel/Makefile
