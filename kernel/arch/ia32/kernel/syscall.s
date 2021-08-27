/* -*- fundamental -*-
 * $Id: syscall.s,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * Lowest level syscall handler
 *
 */
#include <keffos/config.h>
#include <sys/asm.h>

.extern syscall

.text
.globl int_syscall
int_syscall:
	cli
	pusha
	pushl %edx
	pushl %eax
	cld
	call syscall
	addl $8, %esp
	popa
	iret
