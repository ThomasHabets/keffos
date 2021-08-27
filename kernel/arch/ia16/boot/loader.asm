/* -*- fundamental -*-
 * $Id: loader.asm,v 1.3 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * Second stage loader
 *
 */
#include <keffos/config.h>
#include <sys/asm.h>
		
	bits 16

	SECTION .text

global _start
_start:
	jmp past_data

/***************************************************************
	Data
****************************************************************/
sects		db	4

kdrive		db	0
khead		db	0
ksect		db	6 // (1 = bootsect, 2-5 = loader, 6 = kernel)
ktrack		db	0
ksectors	db	0x22 - 5

rdrive		db	0
rhead		db	0
rsect		db	1
rtrack		db	1
rsectors	db	0x22 - 5

max_sector	db	18    // this really ought to be 34 now, I think, maybe
setupmsg	db	'Initilizing kernel...',0
startk_ret	db	'start_kernel() returned. WTF!?! (yeah, bug, bigtime)',0
a20onmsg	db	'Enabling A20...',0
pmodemsg	db	'Going to protected mode and going to kernel entry point',0
pressanymsg	db	'Press any key to continue...',0
readerrmsg	db	'Read error!',0
imageerrmsg	db	'Image broken!',0
lkernelmsg	db	'Loading kernel...',0
lramdrivemsg	db	'Loading ramdrive...',0
okmsg		db	'ok', 0xd, 0xa, 0
textpos		dw	0

/***************************************************************
	Code
***************************************************************/
past_data:
	mov ax, cs
	mov ds, ax
	mov es, ax

	// call detect_mem
	// call detect_equip  // like ps2

	/*
	 * read kernel
	 */
	mov bp, lkernelmsg
	call printstr
	hlt

	push word [kdrive]	// FIXME: drive
	push word [khead]	// head
	push word [ksect]	// sector
	push word [ktrack]	// track
	push word [ksectors]	// sects to read
	push word 0		// offset
	push word KERNEL_SEG	// seg
	push dword CONFIG_KERNEL_SIGNATURE
	call readsectors
	add sp, 0x12

	mov bp, okmsg
	call printstr

	/*
	 *  read ramdrive
	 */
	mov bp, lramdrivemsg
	call printstr

	push word [rdrive]
	push word [rhead]
	push word [rsect]
	push word [rtrack]
	push word [rsectors]
	push word 0
	push word RAMDRIVE_SEG
	push dword CONFIG_RAMDRIVE_SIGNATURE
//	call readsectors
	add sp, 0x12

	mov bp, okmsg
	call printstr
	
	/*
	 * Start of black pmode magic
	 */
	// Enable A20
	mov bp, a20onmsg
	call printstr

	call empty_8042

	mov al, 0xD1
	out 0x64, al		// command write

	call empty_8042

	mov al, 0xDF		// enable A20
	out 0x60, al

	mov bp, okmsg
	call printstr

	/*
	 * init pmode
	 */
	mov bp, pmodemsg
	call printstr	

	/*
	 * save pos on screen
	 */
	mov ah, 0x3
	xor bx, bx
	int 0x10
	mov [textpos], dx

	/*
	 *  Prepare to take a shit over the interrupt tables
	 * i.e. disable interrupts
	 */
	cli			// redundant?
	mov al, 0xFF
	out 0x21, al

	// put GDT in place (0x00000)
	push word 0x0
	pop es
	push cs
	pop ds
	cld
	xor di, di
	mov si, gdt
	mov cx, gdt_size / 4
	rep movsd				// put GDT at 0x00000
	mov cx, (0x10000 - gdt_size) / 4
	mov eax, 0
	rep stosd				// null the rest of the segment
	
	// put IDT in place (0x10000)
	push word 0x1000
	pop es
	xor di, di
	mov eax, 0
	mov cx, 0x200		// IDT is 256 * 8 bytes = 512 * 4 = 0x200 dwords
	rep stosd

	lgdt [cs:gdtr]
	lidt [cs:idtr]

/*
 * In the linux source code it seams linus fears the PIC.
 * It's not that bad, you just need helppc by your side :-)
 *
 * Anyway, we'll do like linux and put the ints at 0x20 - 0x2F, it's a good
 * place.
 *
 * Because I forget what this conflicts with I'll just remind myself:
 *  1) The bios puts ints at 0x08 - 0x0f
 *  2) I think IRQs are mapped at 0x0 -> 0x7 and at 0x70 -> 0x77
 *  3) I think exceptions are mapped at 0x0 -> 0xF
 *  4) So IRQs and exceptions are at the same ints, so IRQs get remapped
 *
 * Documentation in helppc hardware.txt
 */
	// init 8259-1 and 8259-2
	// QUESTION: are these delays needed?
	// ANSWER: Only for compatibility with older PC (old, old, old, old)

	// ICW1
        mov     al,0x11			// 8259 init (ICW4 needed | single 8259 | level triggered)
        out     0x20,al			// 8259-1
        call    delay
        out     0xA0,al			// 8259-2
        call    delay

	// ICW2
        mov     al,0x20                // start of hardware int's (0x20)
        out     0x21,al
        call    delay
        mov     al,0x28                // start of hardware int's 2 (0x28)
        out     0xA1,al
        call    delay

	// ICW3
	// Choose cascading IRQ
        mov     al,00000100b		// IRQ2
        out     0x21,al
        call    delay
        mov     al,00000010b		// IRQ9
        out     0xA1,al
        call    delay

	// ICW4
	// choose some modes
        mov     al,0x01			// 8086 mode
        out     0x21,al
        call    delay
        out     0xA1,al
        call    delay

	// OCW1
	// Turn on IRQs
//        mov     al,0xFF		// service none of the high
	mov	al,00000000b		// service all of the high
        out     0xA1,al			// for 8259-2
        call    delay

//        mov     al,0xFB		// service only irq 2
	mov	al,00000000b		// service all of the low
        out     0x21,al			// for 8259-1

	// QUESTION: forgot OCW2 and OCW3?
	// ANSWER:   shut up, it works

	/*
	 * set protection
	 */
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp short .flush
.flush:


.sigfail:
	call changecolor
.iloop:	jmp .iloop

	/*
	 * black magic end
	 */
//
//
// Functions
//
//

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
 * Nice function to change bg color
 * Good to have if you have problems with selectors
 */
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

/*
 * void readsectors(i32 signature, i16 segment, i16 offset, i8 sects_to_read, i16 track, i16 sector, i16 head, i16 drive)
 *                  2              6              8         A                 C         E          10        12                
 */
readsectors:
	mov bp, sp
	pusha
	push ds

	push ss
	pop ds

	xor ah,ah
	mov dl, [bp + 0xE]
	int 0x13
	
	mov es, [bp + 6]		// segment
	mov al, [bp + 0xA]		// readcount
	mov ah, 0x02			// number
	mov bx, [bp + 8]		// offset
	mov ch, [bp + 0xC]		// track
	mov cl, [bp + 0xE]		// sector
	mov dh, [bp + 0x10]		// head
	mov dl, [bp + 0x12]		// drive
	int 0x13
	jc .loadfail

	jmp .loadedok

.loadfail:
	push cs
	pop es
	mov bp, readerrmsg
	call printstr
.lup:	jmp .lup
.loadedok:
	mov eax, [es:0]
	cmp eax, [bp + 0x2]
	je .goon
	push cs
	pop es
	mov bp, imageerrmsg
	call printstr
.iloop:	hlt
	jmp .iloop
.goon:
	push cs
	pop es

	pop ds
	popa
	mov sp, bp
	ret

//
// wait for keyboard command queue to be emptied
// (shamelessly stolen)
//
empty_8042:     jmp     $+3
                in      al, 0x64        // 8042 status port
                test    al, 1           // output buffer
                jz      .no_output
                jmp     $+3
                in      al, 0x60        // read it
                jmp     empty_8042
.no_output	test    al, 2
                jnz     empty_8042      // yes - loop
                ret
delay:
        dw   0x00eb                  // jmp $+2
        ret
//
// gdt
//
gdt:
descriptor_0:   ; null descriptor. generates GPF on access.
                dw      0
                dw      0
                dw      0
                dw      0
descriptor_1:   ; code segment (exec)
                dw      0xFFFF          ; limit = max
                dw      0x0000          ; low part of base address
                dw      0x9800          ; flags and base
                dw      0x00CF          ; base and flags
descriptor_2:   ; data segment (read/write)
                dw      0xFFFF          ; limit = max
                dw      0x0000          ; low part of base address
                dw      0x9200          ; flags and base
                dw      0x00CF          ; base and flags

gdt_size        equ     $-gdt

idtr:           dw      0x7FF           ; idt limit, 256 IDT entries
                dd      0x00010000      ; idt base (physical addr)

gdtr:           dw      0xFFFF          ; gdt limit, 8192 GDT entries
                dd      0x00000000      ; gdt base (physical addr)

filler		times	0x800-($-$$) db 0
