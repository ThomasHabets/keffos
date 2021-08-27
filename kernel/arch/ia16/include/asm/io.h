/* $Id: io.h,v 1.1 2000/08/08 01:03:39 thompa Exp $ */
#ifndef __INCLUDE_ASM_IO_H__
#define __INCLUDE_ASM_IO_H__

#define outb(value,port) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

__uint8 inb(__uint16 port);

#endif
