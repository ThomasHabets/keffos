/* $Id: int.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * IDT stuff
 *
 */
#include <keffos/config.h>
#include <sys/int.h>
#include <sys/defines.h>
#include <asm/misc.h>
#include <keffos/errno.h>
#include <sys/asm.h>

void restore_inthook(int *saved)
{
	__uint32 a,d,num, flags;
	
	save_flags(flags);
	cli();
	
	a = saved[0];
	d = saved[1];
	num = saved[2];
	
	__asm__ ("movl %%eax, (%%edi)\n\t"
		 "movl %%edx, 0x4(%%edi)\n\t"
		 :
		 : "a" (a),"d" (d),
		 "D" ((__uint32)(0xc0000000 + 0x10000 + (num << 3)))
		);
	
	restore_flags(flags);
}

int *save_inthook(int num)
{
	__uint32 a,d;
	int *pos;
	
	__asm__("movl (%%edi), %%eax\n\t"
		"movl 0x4(%%edi), %%edx\n\t"
		: "=a" (a), "=d" (d)
		: "D" ((__uint32)(0xc0000000 + 0x10000 + (num << 3)))
		);
	if (!(pos = (int*)kmalloc(4 * 3))) {
		errno = ENOMEM;
		return 0;
	}
	pos[0] = a;
	pos[1] = d;
	pos[2] = num;
	return pos;
}

void hookint(int num, int selector, void (*func)(void))
{
	int flags;
	
	// don't even think about using a mutex here
	save_flags(flags);
	cli();
	
	__asm__ ("movl %%eax, (%%edi)\n\t"
		 "movl %%edx, 0x4(%%edi)\n\t"
		 :
		 :
		 "a" ((__uint32)((selector << 16) + ((int)func & 0xffff))),
		 //	   "d" ((__uint32)(((int)func >> 16) + 0x8E00)),
		 "d" ((__uint32)(((int)func & 0xFFFF0000) | 0x8E00)),
		 "D" ((__uint32)(0xc0000000 + 0x10000 + (num << 3)))
		);
	
	restore_flags(flags);
}

/*
 * This is old code (?) We're not using TSS anymore
 */
void settss(int num, void *ptr)
{
	int flags;
	__uint32 p;
	__uint32 value0, value1;
	
#if 0
	value0 = value1 = 0;
	value1 =  (p & 0xFF000000) << 32;
	value1 = (p & 0x00FFFFFF) << 16;
	value |= 0x68;       // limit 104 dec
	value |= 0x808B << 40;
	// G00x L(19..16) PDD0 10B1
	// G = G
	// P = present
	// D = DPL
	// B = busy
	save_flags(flags);
	cli();
	*(__uint64*)(KERNEL_LINEAR_ADDRESS + (num << 3)) = value;
#else
	p = (unsigned int)ptr;
	value0 = ((p & 0x0000FFFF) << 16) | 0x0068;
	value1 = (p &  0xFF000000) | (0x8089 << 8) | ((p & 0x00FF0000) >> 16) ;
	save_flags(flags);
	cli();
	/*
	 *(__uint32*)(KERNEL_LINEAR_ADDRESS + (num << 3)) = value0;
	 *(__uint32*)(KERNEL_LINEAR_ADDRESS + (num << 3) + 4) = value1;
	 */
	__asm__("movl %%eax, (%%edi)\n\t"
		"movl %%ebx, 4(%%edi)\n\t"
		:
		: "a" (value0), "b" (value1),
		"D" (PHYSICAL_LINEAR_ADDRESS + (num << 3)));
#endif
	restore_flags(flags);
	printk("%.8x -> %.8x %.8x\n", p, value1, value0);
}

void copy_gdt_entry(int dest, int src)
{
	int flags;
	save_flags(flags);
	cli();
	*(__uint64*)(PHYSICAL_LINEAR_ADDRESS + (dest << 3)) = 
		*(__uint64*)(PHYSICAL_LINEAR_ADDRESS + (src << 3));
	restore_flags(flags);
}
