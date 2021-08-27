#ifndef __INCLUDE_ASM_IO_H__
#define __INCLUDE_ASM_IO_H__

#define outb(value,port) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

//#define inb(var, port) 
//__asm__ ("inb %%dx,%%al":"=a" (var):"d" (port));
unsigned char inb(unsigned short port);

#endif
