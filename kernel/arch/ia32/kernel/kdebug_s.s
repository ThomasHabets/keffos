/* -*- fundamental -*-
 * $Id: kdebug_s.s,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/asm.h>

.extern kdebug_keybbhandler

.text
.globl kdebug_int_timer
kdebug_int_timer:
	cli
	pushl %eax

	movb $0x20, %al
	outb %al, $0x20

	popl %eax
	iret	


.globl kdebug_int_keyb
kdebug_int_keyb:
	cli

	pushl %eax

	xor %eax, %eax
	inb $0x60, %al

	// if esc, reboot in a very bad way
	cmpb $0x1, %al
	jne .noreboot
	pushw $0
	popw %ds
	movl %eax, (0)
.noreboot:	
	pushw %ax
	cld		// I think gcc expects this
	call kdebug_keybhandler
	addl $2, %esp

	popl %eax

	movb $0x20, %al
	outb %al,$0x20
	sti
	iret
