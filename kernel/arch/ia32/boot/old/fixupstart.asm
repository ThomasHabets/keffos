/* -*- fundamental -*- */

#include <keffos/config.h>
#include <sys/asm.h>

	bits 32

signarure	db CONFIG_KERNEL_SIGNATURE
checksum32	dd 0
kernel_size	dd 0

global _start
extern fixup, fixupend

_start:
	call fixup
	jmp fixupend + 128
.hlt	jmp .hlt
