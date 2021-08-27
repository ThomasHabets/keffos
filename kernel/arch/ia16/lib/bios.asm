# -*- fundamental -*-
# $Id: bios.asm,v 1.2 2001/05/31 21:32:21 marvin Exp $
#
	global	__io_printchar
__io_printchar
	push ax
	push bx

	mov ah, 0xe
	mov al, [bp - 8]
	mov bx, 0x0007
	int 0x10

	pop bx
	pop ax
	ret

global	__io_printnstr
__io_printnstr
	push ax
	push bx
	push cx
	push dx
	push bp

	mov ah, 0x3
	xor bh,bh
	int 0x10

	mov ax, 0x1301
	mov bx, 0x0007
	mov cx, [bp - 8]
	mov bp, [bp - 10]
	int 0x10
	
	pop bp
	pop dx
	pop cx
	pop bx
	pop ax
	ret

global __io_printnl
__io_printnl
	push ax
	mov ax, 0xe0d
	int 0x10
	mov al, 0xa
	int 0x10
	pop ax
	ret

global __rm_getch
__rm_getch
	xor ax,ax
	int 0x16
	mov ah, al
	mov al, 0
	ret
	
global __cli
__cli
	cli
	ret

global __hlt
__hlt
	hlt
	ret
	
global __sti
__sti
	sti
	ret