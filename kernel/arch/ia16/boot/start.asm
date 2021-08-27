/* -*- fundamental -*-
 * $Id: start.asm,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * First code exectuted in actual pmode, also its handled
 * by ld and not cat. That's cool.
 * File entry point is 0xc0030000 and _start is assumed to be @ 0xC003000C
 *
 */
#include <keffos/config.h>
#include <sys/asm.h>

	bits 16

extern start_kernel,panic
global checksum32,kernel_size,signature
signature	db CONFIG_KERNEL_SIGNATURE

	// FIXME: change to md5? that would be cool
	// NOTE: if you change here you must also change CODESTART
	// in the Makefile
checksum32	dd 0
kernel_size	dd 0
	
global _start
_start:
	hlt


changecolor:
	mov al, 0
	mov dx, 0x3c8
	out dx, al
	
	mov dx, 0x3c9
	mov al, 60
	out dx, al
	out dx, al
	out dx, al
	ret

section .data
this_cant_happen db 'Start_kernel returned (now in asm code)', 0
nocontinue	db 'Can not continue anymore',0
gdtr		dw 0xFFFF
		dd GDT_LINEAR_ADDRESS
idtr		dw 0x7FF
		dd IDT_LINEAR_ADDRESS
