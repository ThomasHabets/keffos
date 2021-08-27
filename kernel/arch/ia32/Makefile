# $Id: Makefile,v 1.2 2001/05/31 21:32:21 marvin Exp $
#
# Used by jump from loader.asm to start.asm
#
CODESTART=0xC003000C		# 4 magic + 4 checksum + 4 size
PARMSIZE=128

ARCHDIR=arch/ia32

#-------------------------------------#
TARGET=vmkeffos
#-------------------------------------#
CC=gcc -Os
#CFLAGS=-Werror $(INCLUDES) -g $(DEFINES) -fomit-frame-pointer -pipe -Wall -w # -O2
#CFLAGS=-Os
K_CFLAGS=\
 -DCODESTART=$(CODESTART) -DPARMSIZE=$(PARMSIZE) -Os

LD=ld
LDFLAGS=-oformat=binary -s -N -x -M -Ttext C0030000

CPP=cpp
CPPFLAGS=-nostdinc -nostdinc++ $(DEFINES) $(INCLUDES) #-pedantic -Wall

ASM=nasm
ASMFLAGS=

ATTAS=as
ATTASFLAGS=

# FIXME: replace with -Wa,aln=$@.s or something
TOASM=echo $(CC) $(CFLAGS) $(K_CFLAGS) -S > /dev/null
#----------------------------------#
KERNEL_FILES_ARCH=\
$(ARCHDIR)/boot/start.o\
$(ARCHDIR)/kernel/console.o\
$(ARCHDIR)/kernel/cpuid.o\
$(ARCHDIR)/kernel/exception.o\
$(ARCHDIR)/kernel/int.o\
$(ARCHDIR)/kernel/inthandlers.o\
$(ARCHDIR)/kernel/kdebug_s.o\
$(ARCHDIR)/kernel/setup.o\
$(ARCHDIR)/kernel/shutdown.o\
$(ARCHDIR)/kernel/syscall.o\
$(ARCHDIR)/lib/bios.o\
$(ARCHDIR)/mm/paging.o

KERNEL_FILES_ALL=\
kernel/main.o\
kernel/ramdrive.o\
kernel/vfs.o\
kernel/module.o\
kernel/init.o\
kernel/kdebug.o\
kernel/klog.o\
kernel/mutex.o\
kernel/symbols.o\
kernel/syscall.o\
kernel/timer.o\
lib/ctype.o\
lib/errno.o\
lib/stdio.o\
lib/string.o\
lib/checksum.o\
lib/vsnprintf.o\
mm/mm.o\
mm/kmm.o

KERNEL_FILES=$(KERNEL_FILES_ARCH) $(KERNEL_FILES_ALL)

#-------------------------------#
all: include/keffos/config.h image

include/asm include/sys:
	ln -sf $(shell pwd)/$(ARCHDIR)/include/asm include/asm
	ln -sf $(shell pwd)/$(ARCHDIR)/include include/sys

$(ARCHDIR)/boot/%.nasm : $(ARCHDIR)/boot/%.asm
	$(CPP) $(CPPFLAGS) $(K_CFLAGS) < $< > $@
$(ARCHDIR)/boot/%.o : $(ARCHDIR)/boot/%.nasm
	$(ASM) $(ASMFLAGS) -f elf -o $@ $<

$(ARCHDIR)/kernel/%.o : $(ARCHDIR)/kernel/%.c
	$(CC) $(CFLAGS) $(K_CFLAGS) -c -o $@ $<
	$(TOASM) $(CFLAGS) $(K_CFLAGS) -c -o $<.s $<
$(ARCHDIR)/kernel/%.o :  $(ARCHDIR)/kernel/%.s
	$(CPP) $(CPPFLAGS) $(K_CFLAGS) < $< | $(ATTAS) $(ATTASFLAGS) -o $@

$(ARCHDIR)/lib/%.nasm :  $(ARCHDIR)/lib/%.asm
	$(CPP) $(CPPFLAGS) $(K_CFLAGS) < $< > $@
$(ARCHDIR)/lib/%.o :  $(ARCHDIR)/lib/%.nasm
	$(ASM) $(ASMFLAGS) -f elf -l $@.lst -o $@ $<

lib/%.o : lib/%.c
	$(CC) $(CFLAGS) $(K_CFLAGS) -c -o $@ $<
	$(TOASM) $(CFLAGS) $(K_CFLAGS) -c -o $<.s $<

kernel/%.o:  kernel/%.c
	$(CC) $(CFLAGS) $(K_CFLAGS) -c -o $@ $<
	$(TOASM) $(CFLAGS) $(K_CFLAGS) -c -o $<.s $<
#-------------------------------#
tools/mkramfs tools/sysmap tools/postmake:
	make -C tools
#-------------------------------#
include/keffos/config.h: 
	make -C .. kernel/include/keffos/config.h
#-------------------------------#
bootsect:  $(ARCHDIR)/boot/boot.nasm
	$(ASM) $(ASMFLAGS) -f bin -o $@ -l $@.lst $<

System.map: System.map.in tools/sysmap
	tools/sysmap < System.map.in > System.map

$(TARGET) System.map.in: include/asm include/sys $(KERNEL_FILES) tools/postmake
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_FILES) > System.map.in
#	cp $(TARGET) vmkeffoz
#	nm vmkeffos | sort > System.map
#	tools/postmake $(TARGET)

loader:  $(ARCHDIR)/boot/loader.nasm
	$(ASM) $(ASMFLAGS) -f bin -o $@ -l $@.lst $<

ramdrive: tools/mkramfs System.map
	tools/mkramfs $@ System.map

image:  include/asm include/sys include/keffos/config.h bootsect loader \
$(TARGET) ramdrive
	cat bootsect loader vmkeffos > $@


disk:  floppy.img
	cat floppy.img > /dev/fd0

floppy.img: image
	dd if=image of=floppy.img bs=512 seek=0
	dd if=ramdrive of=floppy.img bs=512 seek=36 count=20

#-------------------------------
clean: 
	rm -f $(KERNEL_FILES) ramdrive image bootsect kboot include/config.h \
loader $(TARGET) betadisc System.map.in System.map include/asm include/sys \
`find -name "*.lst" -o -name "*.o" -o -name "*.nasm" -o -name "*.c.s"` \
floppy.img
	make -C tools clean
