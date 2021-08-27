	bits 16

global _main
_main
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov bp, loading
	mov cx, loading_len
	call printstr
	call printnl
	
	hlt

loading		db 'Loading microkernel...'
loading_len	equ $ - loading

printstr
	push cx
	mov ah, 0x3
	xor bh,bh
	int 0x10
	pop cx
	mov ax, 0x1301
	mov bx, 0x0007
	int 0x10
	ret

printnl
	mov ax, 0xe0d
	int 0x10
	mov al, 0xa
	int 0x10
	ret