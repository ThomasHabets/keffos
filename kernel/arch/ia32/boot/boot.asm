/* -*- fundamental -*-
 * $Id: boot.asm,v 1.3 2001/05/31 21:32:21 marvin Exp $
 *
 * This is the boot sector for both ia32 and ia16 kernels.
 *
 * According to designs as of feb 22, 1999 this is what it should do:
 *  1) Move self elsewhere (INITSEG)
 *  2) Set 80x50 if CONFIG_KERNEL_SCREEN_80x50 is 1
 *  3) print "Bootloader %d.%d\nboot> "
 *  4) get boot dev (or label) and parameters
 *  5) print "Loading kernel loader...%d sectors"
 *  6) load first sector of loader (alt. load boot sector of a drive)
 *  7) get sectorcount from that sector and load rest
 *  8) jmp to loader (alt: boot sector)
 *
 * Check Documentation/boot.txt for up-to-date info.
 *
 * Started by Thomas
 * Changed 1999-03-28  by Max
 *  * Changed all output to use the in bios tty support removing most needs
 *    of printnl and manipulating cursor movment. Also implemented ASCIIZ
 *    which reduced the size of the bootsector A LOT. 
 *    (wonder why linux doesn't do it this way,..)
 *  * Added reinitialization of the floppy. (now it is ok to remove and 
 *    reinstert the floppy.
 *  * Did som general optimizing (like removing redundant register loads...)
 * Changed 1999-04-10  by Max
 *  * Fixed sector count bug, the specification lied.
 */
#include <keffos/config.h>
#include <sys/asm.h>

#define SAVE_BYTES 0
#define BOOT_PROMPT_LEN 126
	
	bits 16

	SECTION .text

global  _main		// Needed?
_main:			// Don't know, don't care

//----------------------------------------------------
// MOVE CODE!!! DO NOT USE ANYTHING RELATIVE HERE
//----------------------------------------------------
	mov ax, BOOTSEG
	mov ds, ax
	mov ax, INITSEG
	mov es, ax

	mov ax, STACKSEG
	mov ss, ax

	xor di, di
	xor si, si
	mov cx, 0x80
	cld
	rep movsd
	
	jmp INITSEG:go
//------------------------------------------------
//             Data
//------------------------------------------------
//		             1         2         3
//		    123456789012345678901234567890
idstr		db 'KeffOSRockarFett',0
bootdrv		db 0
banner		db 'Bootloader 0.1'
twonl		db 0xD,0xA,0xA,0
prompt		db 'boot> ',0
loading		db 'Loading second stage loader...',0
errbang		db 'Error!',0xD,0xA,0
sectors		db ' sectors',0

//------------------------------------------------
//             Init code running in INITSEG
//------------------------------------------------
go:
	mov ax, INITSEG
	mov ds, ax
	mov es, ax

	mov [bootdrv], dl

#if CONFIG_KERNEL_SCREEN_80x50
	mov ax, 0x0003		//Initialize 80x50
	int 0x10
	mov ax, 0x83
	int 0x10
	mov ax, 0x1112
	mov bl, 0
	int 0x10                //load 8x8 ROM font
#endif
	mov bp, banner		// Bootloader %d.%d
	call printstr
	mov bp, prompt		// boot> 
	call printstr
	
//	push word MEM_PARMSEG	// FIXME:
//	pop fs			//   space is allocated in kernel image for
//	call read		//  this crap.

/*
 * Got parameters
 */
#if 0 // load DOS from HD      UPDATE: do this in second stage loader
	mov al, '0'
	cmp [fs:0], al
	jne goon
	
	push word 0
	pop es
	mov ax, 0x0201		// read one sector
	mov bx, 0x7c00
	xor ch,ch
	xor cl,cl
	mov dl, 80
	xor dh,dh
	int 0x13
	
	// set up for call
	xor ax,ax
	xor bx,bx
	xor cx,cx
	xor dx,dx

	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0xfffe
	
	mov dl,80
	jmp 0x7c00:0

goon:
#endif
	mov bp, twonl
	call printstr
	mov bp, loading
	call printstr

/*
 *	First load one sector and check how many should be loaded
 */
loadk:
	push word KERNEL_LOADER_SEG
	pop es
	mov bx, 0

	mov ah, 0		// reset disk system (reinitialization)
	mov dl, [bootdrv]	// drive
	int 0x13

	mov ax, 0x0201		// read 1 sector
	xor cx, cx		// track/cylinder number (0-1024)
	mov cl, 2		// sector number (1-17)
	xor dh, dh		// head number (0-15)
	int 0x13
	jc errorload

	// ax gets clobbered

	mov ah, 0x02
	mov al, [es:3]		// the sector count place
	int 0x13
	jc errorload

	call kill_motor

	mov al, [es:3]		// according to the specificationm al should
	add al, 0x30		// already contain the number of sectors, but
				// it doesn't
	call printchr
#if !SAVE_BYTES
	push word INITSEG
	pop es	
	mov bp, sectors
	call printstr
#endif
	jmp KERNEL_LOADER_SEG:0		// second stage loader

errorload:
	call kill_motor
	mov bp, errbang
	call printstr
	jmp INITSEG:go		// Go back to the prompt
	
/*
 * read a string at the current cursor position (with echo) using BIOS calls
 * string will be in FS:BP, max length is defined at compiletime
 * ptr fixed to FS:0
 *
 */
read:           xor     bp, bp          ; zero offset in string
.readloop:      mov     ah, 0x10
                int     0x16            ; read key from keyboard
                                
                cmp     al, 8           ; Backspace
                jnz     .check_for_enter
                cmp     bp, 0
                je      .readloop
                mov     byte [fs:bp], 0
                dec     bp
	
                mov     ah, 0x3
                mov     bh, 0
                int     0x10            ; read cursor position
		cmp	dl, 0
		jnz	.nonewline
		
                mov     ah, 0x2
		dec	dh			
		mov	dl, 0x4f
                int     0x10            ; set cursor position
		push	ax
		mov	al, ' '
		call	printchr
		pop	ax
		int	0x10		; set cursor position
		jmp	.readloop	
.nonewline:
		call	printchr
		mov	al, ' '
		call	printchr
		mov	al, 8
		call	printchr
                jmp     .readloop
.check_for_enter:
                cmp     al, 0x0D        ; Enter
                je      .done
                cmp     bp, BOOT_PROMPT_LEN
                je      .readloop

                mov     [fs:bp], al
                call	printchr
                inc     bp
                jmp     .readloop

.done:          mov     byte [fs:bp+1], 0 ; null termination
                xor	bp, bp
		ret

/*
 * print ASCIIZ string, ptr in bp
 */
printstr:
.strloop:
	mov al, [es:bp]
	and al, al
	jz  .strdone
	call printchr
	inc bp
	jmp .strloop
.strdone:
	ret

/*
 * print char in all using in bios tty function
 */
printchr:
	mov ah, 0xE
	xor bx, bx
	int 0x10
	ret

/*
 * kill-floppy-motor routine shamelessly stolen from linux
 */
kill_motor:
        push dx
        mov dx, 0x3f2
        mov al, 0
        outb
        pop dx
        ret
	
filler		times	510-($-$$) db 0
boot_signature	dw	0xAA55
