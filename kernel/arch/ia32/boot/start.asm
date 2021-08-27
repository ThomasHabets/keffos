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

	bits 32

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
	mov al, '3'
	mov [SCREEN_LINEAR_ADDRESS], al

	call setup_idt
	jmp .nexx
.nexx:

	/*
	 * GDTR and IDTR presently point to
	 * memory in PDE 0x0.
	 * Change to PDE 0x300
	 */ 
	lidt [idtr]
	lgdt [gdtr]

	/*
	 * Ok, PDE 0x0 is no longer needed
	 */
	mov eax, 0x11000
	mov dword [eax], 0
	mov cr3, eax
	jmp .goon
.goon:

	mov al, 'a'
	mov [SCREEN_LINEAR_ADDRESS], al

	call start_kernel	// jump to C code

	push long this_cant_happen
	call panic
.nocont:
	push long nocontinue
	call panic
	jmp .nocont
	ret
	
align 16
global ignore_int
ignore_int
	cli

	push eax

	mov al, [SCREEN_LINEAR_ADDRESS]
	inc al
	mov [SCREEN_LINEAR_ADDRESS], al

	pop eax
	mov al, 0x20
	out 0x20,al
//	add esp, 4
//	hlt
//	sti
	iretd

setup_idt
	cli

        mov edx, ignore_int
	mov eax, KERNEL_CODE_SELECTOR
	shl eax, 16
        mov ax, dx            /* selector = 0x0002 = cs */
        mov dx, 0x8E00        /* interrupt gate - dpl=0, present */

	mov ecx, 0x100
.hookint:
	dec ecx
	mov edi, ecx
	shl edi, 3

	add edi, IDT_LINEAR_ADDRESS

	mov [edi], eax
	mov [edi + 4], edx
	cmp ecx, 0
	jne .hookint
	ret

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
