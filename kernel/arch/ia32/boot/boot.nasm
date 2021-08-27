# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "<stdin>"
# 30 "<stdin>"
# 1 "/home/thompa/scm/keffos/kernel/include/keffos/config.h" 1
# 31 "<stdin>" 2
# 1 "/home/thompa/scm/keffos/kernel/include/sys/asm.h" 1
# 32 "<stdin>" 2




 bits 16

 SECTION .text

global _main
_main:




 mov ax, 0x07C0
 mov ds, ax
 mov ax, 0x1000
 mov es, ax

 mov ax, 0x9000
 mov ss, ax

 xor di, di
 xor si, si
 mov cx, 0x80
 cld
 rep movsd

 jmp 0x1000:go





idstr db 'KeffOSRockarFett',0
bootdrv db 0
banner db 'Bootloader 0.1'
twonl db 0xD,0xA,0xA,0
prompt db 'boot> ',0
loading db 'Loading second stage loader...',0
errbang db 'Error!',0xD,0xA,0
sectors db ' sectors',0




go:
 mov ax, 0x1000
 mov ds, ax
 mov es, ax

 mov [bootdrv], dl


 mov ax, 0x0003
 int 0x10
 mov ax, 0x83
 int 0x10
 mov ax, 0x1112
 mov bl, 0
 int 0x10

 mov bp, banner
 call printstr
 mov bp, prompt
 call printstr
# 137 "<stdin>"
 mov bp, twonl
 call printstr
 mov bp, loading
 call printstr




loadk:
 push word 0x8000
 pop es
 mov bx, 0

 mov ah, 0
 mov dl, [bootdrv]
 int 0x13

 mov ax, 0x0201
 xor cx, cx
 mov cl, 2
 xor dh, dh
 int 0x13
 jc errorload



 mov ah, 0x02
 mov al, [es:3]
 int 0x13
 jc errorload

 call kill_motor

 mov al, [es:3]
 add al, 0x30

 call printchr

 push word 0x1000
 pop es
 mov bp, sectors
 call printstr

 jmp 0x8000:0

errorload:
 call kill_motor
 mov bp, errbang
 call printstr
 jmp 0x1000:go







read: xor bp, bp ; zero offset in string
.readloop: mov ah, 0x10
                int 0x16 ; read key from keyboard

                cmp al, 8 ; Backspace
                jnz .check_for_enter
                cmp bp, 0
                je .readloop
                mov byte [fs:bp], 0
                dec bp

                mov ah, 0x3
                mov bh, 0
                int 0x10 ; read cursor position
  cmp dl, 0
  jnz .nonewline

                mov ah, 0x2
  dec dh
  mov dl, 0x4f
                int 0x10 ; set cursor position
  push ax
  mov al, ' '
  call printchr
  pop ax
  int 0x10 ; set cursor position
  jmp .readloop
.nonewline:
  call printchr
  mov al, ' '
  call printchr
  mov al, 8
  call printchr
                jmp .readloop
.check_for_enter:
                cmp al, 0x0D ; Enter
                je .done
                cmp bp, 126
                je .readloop

                mov [fs:bp], al
                call printchr
                inc bp
                jmp .readloop

.done: mov byte [fs:bp+1], 0 ; null termination
                xor bp, bp
  ret




printstr:
.strloop:
 mov al, [es:bp]
 and al, al
 jz .strdone
 call printchr
 inc bp
 jmp .strloop
.strdone:
 ret




printchr:
 mov ah, 0xE
 xor bx, bx
 int 0x10
 ret




kill_motor:
        push dx
        mov dx, 0x3f2
        mov al, 0
        outb
        pop dx
        ret

filler times 510-($-$$) db 0
boot_signature dw 0xAA55
