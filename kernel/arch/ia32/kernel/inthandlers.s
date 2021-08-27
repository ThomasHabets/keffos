/* -*- fundamental -*-
 * $Id: inthandlers.s,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * asm stuff for internal cpu exceptions
 *
 */
#include <keffos/config.h>
#include <sys/asm.h>

#define CODE_ALIGN 2

.extern printk, keybhandler, panic, default_c_exception_handler,timerhandler

#define START_INT \
	cli; \
	SAVE_REGS

#define SAVE_REGS \
	pushw %ds; \
	pushw %es; \
	pushw %ss; \
	pushw %fs; \
	pushw %gs; \
	pushl %eax; \
	pushl %ebx; \
	pushl %ecx; \
	pushl %edx; \
	pushl %esi; \
	pushl %edi; \
	pushl %ebp; \
	movl %cr3, %eax; \
	pushl %eax

#define RESTORE_REGS \
	popl %eax; \
	movl %eax, %cr3; \
	popl %ebp; \
	popl %edi; \
	popl %esi; \
	popl %edx; \
	popl %ecx; \
	popl %ebx; \
	popl %eax; \
	popw %gs; \
	popw %fs; \
	popw %ss; \
	popw %es; \
	popw %ds


.text
.globl testint
testint:
	movl $0x4711, %eax
	iret
#if 0
#define EXCEPTION(INTR) \
.text ; \
.align CODE_ALIGN, 0x90; \
	cli ; \
	movl %esp, %eax ;\
	pushl %eax ; \
	pushl $ INTR ; \
	call default_c_exception_handler; \
	addl $8, %esp; \
	movb $0x20, %al; \
	outb %al,$0x20; \
	sti; \
	iret;
#else
	#define EXCEPTION(INTR) \
.text ; \
.align CODE_ALIGN, 0x90; \
	cli; \
	movl %esp, %eax; \
	pusha ; \
	pushl %eax; \
	pushl $ INTR; \
	jmp infloop;

infloop:
	call default_c_exception_handler
	addl $8, %esp
	popa
	iret

#endif

.globl exception0; exception0: ; EXCEPTION(0x0)
.globl exception1; exception1: ; EXCEPTION(0x1)
.globl exception2; exception2: ; EXCEPTION(0x2)
.globl exception3; exception3: ; EXCEPTION(0x3)
.globl exception4; exception4: ; EXCEPTION(0x4)
.globl exception5; exception5: ; EXCEPTION(0x5)
.globl exception6; exception6: ; EXCEPTION(0x6)
.globl exception7; exception7: ; EXCEPTION(0x7)
.globl exception8; exception8: ; EXCEPTION(0x8)
.globl exception9; exception9: ; EXCEPTION(0x9)
.globl exceptiona; exceptiona: ; EXCEPTION(0xA)
.globl exceptionb; exceptionb: ; EXCEPTION(0xB)
.globl exceptionc; exceptionc: ; EXCEPTION(0xC)
.globl exceptiond; exceptiond: ; EXCEPTION(0xD)
.globl exceptione; exceptione: ; EXCEPTION(0xE)
.globl exceptionf; exceptionf: ; EXCEPTION(0xF)

.text
.globl int_keyb
int_keyb:
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
	cmpb $0x2, %al
	jne .nokdebug
	movb $0x20, %al
	outb %al,$0x20
	call kdebug
	popl %eax
	iret
	
.nokdebug:
	pushw %ax
	cld		// I think gcc expects this
	call keybhandler
	addl $2, %esp

	movb $0x20, %al
	outb %al,$0x20
	popl %eax
	iret

.text
.global schedule
schedule:
	SAVE_REGS
	cld
	movl %esp, %eax
	pushl %eax
	call next_process
	movl %eax, %esp
	RESTORE_REGS
	ret

.text
.global int_timer
int_timer:
	cli
	call schedule
	pushl %eax
	movb $0x20, %al
	outb %al, $0x20
	popl %eax
	iret

exit_int:
	cld			// gcc expects this
	movl %esp, %eax
	pushl %eax
	call next_process
	movl %eax, %esp

exit_int_no_schedule:
	RESTORE_REGS
	pushl %eax
	movb $0x20, %al
	outb %al, $0x20
	popl %eax
	iret

.text
.global exception_mem_page_fault
.extern got_pagefault
exception_mem_page_fault:
	pusha
	movb $1, %ah
	movb %ah, got_pagefault
	popa
	iret
