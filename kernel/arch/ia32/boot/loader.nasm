# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "<stdin>"
# 9 "<stdin>"
# 1 "/home/thompa/scm/keffos/kernel/include/keffos/config.h" 1
# 10 "<stdin>" 2
# 1 "/home/thompa/scm/keffos/kernel/include/sys/asm.h" 1
# 11 "<stdin>" 2

 bits 16

 SECTION .text

global _start
_start:
 jmp past_data




sects db 4


kdrive db 0
khead db 0
ksect db 6
ktrack db 0
ksectors db 0x22 - 5

rdrive db 0
rhead db 0
rsect db 1
rtrack db 1
rsectors db 0x22 - 5

max_sector db 18
setupmsg db 'Initilizing kernel 0.0.1...',0
startk_ret db 'start_kernel() returned. WTF!?! (yeah, bug, bigtime)',0
no386msg db 'Your computer sucks, compile the 8086 version',0
is386msg db 0xd,0xa,'Processor is 80386 or better, cool',0xd,0xa,0
a20onmsg db 'Enabling A20...',0
pmodemsg db 'Going to protected mode and going to kernel entry point',0xd,0xa,0
pressanymsg db 'Press any key to continue...',0
readerrmsg db 'Read error!',0
imageerrmsg db 'Image broken!',0
lkernelmsg db 'Loading kernel...',0
lramdrivemsg db 'Loading ramdrive...',0
okmsg db 'ok', 0xd, 0xa, 0
textpos dw 0




past_data:
 mov ax, cs
 mov ds, ax
 mov es, ax

 call check_cpu






 mov bp, lkernelmsg
 call printstr

 push word [kdrive]
 push word [khead]
 push word [ksect]
 push word [ktrack]
 push word [ksectors]
 push word 0
 push word 0x3000
 push dword 'Keff'
 call readsectors
 add sp, 0x12

 mov bp, okmsg
 call printstr




 mov bp, lramdrivemsg
 call printstr

 push word [rdrive]
 push word [rhead]
 push word [rsect]
 push word [rtrack]
 push word [rsectors]
 push word 0
 push word 0x7000
 push dword 'KoRD'

 add sp, 0x12

 mov bp, okmsg
 call printstr





 mov bp, a20onmsg
 call printstr
 jmp go32bitnowlolz
 call empty_8042

 mov al, 0xD1
 out 0x64, al

 call empty_8042

 mov al, 0xDF
 out 0x60, al

 mov bp, okmsg
 call printstr




go32bitnowlolz:
 mov bp, pmodemsg
 call printstr




 mov ah, 0x3
 xor bx, bx
 int 0x10
 mov [textpos], dx





 cli
 mov al, 0xFF
 out 0x21, al


 push word 0x0
 pop es
 push cs
 pop ds
 cld
 xor di, di
 mov si, gdt
 mov cx, gdt_size / 4
 rep movsd
 mov cx, (0x10000 - gdt_size) / 4
 mov eax, 0
 rep stosd


 push word 0x1000
 pop es
 xor di, di
 mov eax, 0
 mov cx, 0x200
 rep stosd

 lgdt [cs:gdtr]
 lidt [cs:idtr]
# 193 "<stdin>"
        mov al,0x11
        out 0x20,al
        call delay
        out 0xA0,al
        call delay


        mov al,0x20
        out 0x21,al
        call delay
        mov al,0x28
        out 0xA1,al
        call delay



        mov al,00000100b
        out 0x21,al
        call delay
        mov al,00000010b
        out 0xA1,al
        call delay



        mov al,0x01
        out 0x21,al
        call delay
        out 0xA1,al
        call delay




 mov al,00000000b
        out 0xA1,al
        call delay


 mov al,00000000b
        out 0x21,al







 mov eax, cr0
 or eax, 1
 mov cr0, eax
 jmp short .flush
.flush:





 mov ax, 2 << 3
  mov ss, ax
 mov esp, 0xFFFE



 mov ax, 2 << 3
 mov ds, ax
 mov es, ax
 mov fs, ax
 mov gs, ax

 mov ax, '1'
 mov [(0x000b8000)], al

 mov eax, [ds:dword (0x00030000)]
 cmp eax, 'Keff'
 jmp goon
 mov ax, '/'
 mov [(0x000b8000)], al
 mov ax, 'M'
 mov [(0x000b8000) + 2], al
 hlt

goon:
 mov ax, '2'
 mov [(0x000b8000)], al
# 286 "<stdin>"
 cli

 mov ebx, 0x11000
 mov dword [ebx], 0x12000 + 1
 add ebx, 4 * 0x300
 mov dword [ebx], 0x13000 + 1




 mov ecx, 0
 mov ebx, 0x12000
 mov edx, 0x13000
 mov eax, 1 | 2
.loop:
 mov [ebx], eax
 mov [edx], eax
 add ebx, 4
 add edx, 4
 add eax, 0x1000
 inc ecx
 cmp ecx, 0x400
 jne .loop

 mov eax, 0x11000
 mov cr3, eax
 mov eax, cr0
 or eax, 0x80000000
 mov cr0, eax
 jmp short .flush
.flush:
 mov eax, [dword ((0xc0000000)+(0x00030000))]
 cmp eax, 'Keff'


 mov esp, ((0xc0000000) + 0xfffe)
 jmp dword 1 << 3:0xC003000C

.sigfail:
 call changecolor
.iloop: jmp .iloop
# 340 "<stdin>"
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




check_cpu:
 pushf
 xor ah,ah
 push ax
 popf
 pushf
 pop ax
 and ah, 0xF0
 cmp ah, 0xF0
 je .no386

 mov ah, 0x70
 push ax
 popf
 pushf
 pop ax
 and ah, 0x70
 jz .no386

 popf
 mov bp, is386msg
 call printstr
 ret
.no386:
 mov bp, no386msg
 call printstr
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





readsectors:
 mov bp, sp
 pusha
 push ds

 push ss
 pop ds

 xor ah,ah
 mov dl, [bp + 0xE]
 int 0x13

 mov es, [bp + 6]
 mov al, [bp + 0xA]
 mov ah, 0x02
 mov bx, [bp + 8]
 mov ch, [bp + 0xC]
 mov cl, [bp + 0xE]
 mov dh, [bp + 0x10]
 mov dl, [bp + 0x12]
 int 0x13
 jc .loadfail

 jmp .loadedok

.loadfail:
 push cs
 pop es
 mov bp, readerrmsg
 call printstr
.lup: jmp .lup
.loadedok:
 mov eax, [es:0]
 cmp eax, [bp + 0x2]
 jne .goon
 push cs
 pop es
 mov bp, imageerrmsg
 call printstr
.iloop: hlt
 jmp .iloop
.goon:
 push cs
 pop es

 pop ds
 popa
 mov sp, bp
 ret





empty_8042: jmp $+3
                in al, 0x64
                test al, 1
                jz .no_output
                jmp $+3
                in al, 0x60
                jmp empty_8042
.no_output test al, 2
                jnz empty_8042
                ret
delay:
        dw 0x00eb
        ret



gdt:
descriptor_0: ; null descriptor. generates GPF on access.
                dw 0
                dw 0
                dw 0
                dw 0
descriptor_1: ; code segment (exec)
                dw 0xFFFF ; limit = max
                dw 0x0000 ; low part of base address
                dw 0x9800 ; flags and base
                dw 0x00CF ; base and flags
descriptor_2: ; data segment (read/write)
                dw 0xFFFF ; limit = max
                dw 0x0000 ; low part of base address
                dw 0x9200 ; flags and base
                dw 0x00CF ; base and flags

gdt_size equ $-gdt

idtr: dw 0x7FF ; idt limit, 256 IDT entries
                dd 0x00010000 ; idt base (physical addr)

gdtr: dw 0xFFFF ; gdt limit, 8192 GDT entries
                dd 0x00000000 ; gdt base (physical addr)

filler times 0x800-($-$$) db 0
