/* $Id: setup.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * Arch-dependant setup
 *
 */
#include <keffos/config.h>
#include <keffos/setup.h>
#include <keffos/syscall.h>
#include <sys/asm.h>
#include <sys/int.h>
#include <asm/misc.h>

#pragma pack(1)
typedef struct ia32_tss_s {

	// set to 0
	__uint16 link, unused0;

	// set to the end of the task segment page
	__uint32 esp0;

	// set to SEL_KDATA (Kernel Data segment)
	__uint16 ss0, unused1;

	// set to 0
	__uint32 esp1;

	// set to 0
	__uint16 ss1, unused2;

	// set to 0
	__uint32 esp2;

	// set to 0
	__uint16 ss2, unused3;


	// set to the physical address of this tasks page tables
	__uint32 cr3;

	// set to the entry point to this tasks code
	__uint32 eip;

	// set to 0x4202
	__uint32 eflags;
	
        // set to garbage values
	__uint32 eax, ecx, edx, ebx, esp, ebp, esi, edi;

	// set to SEL_KDATA (Kernel data segment)
	__uint16 es,  unused4;

	// set to SEL_KCODE (Kernel code segment)
	__uint16 cs,  unused5;

	// set to SEL_KDATA
	__uint16 ss,  unused6;

	// set to SEL_KDATA
	__uint16 ds,  unused7;

	// set to SEL_KDATA
	__uint16 fs,  unused8;

	// set to SEL_KDATA
	__uint16 gs,  unused9;

	// set to 0
	__uint16 ldt, unused10;

	// set to 0
	__uint16 debugtrap;

	// set to 0
	__uint16 iomapbase;
} ia32_tss_t;
#pragma unpack


void testinthook(void)
{
	int c;
	hookint(0x80, KERNEL_CODE_SELECTOR, testint);
	__asm__("int $0x80"
		: "=a" (c)
		: "a" (0));
	if (c == 0x4711) {
		printk("ok\n");
	} else {
		printk("what? hooking failed? And no reboot?!\n");
		panic("Interrupt hooking failed");
	}
}

volatile char got_pagefault = 0;
void exception_mem_page_fault(void);

unsigned char inb(unsigned short port)
{
	unsigned char ret;
	__asm__ ("inb %%dx,%%al":"=a" (ret):"d" (port));
	return ret;
}

/*
 * All interrupts are now set to int_ignore() so 
 * just set the ones you want
 */
void setup_arch(void)
{
	int f = 54;
	__uint32 c;
	save_flags(f);
	/*
	  #define PAGE_SIZE 4096
	  hookint(0xE, KERNEL_CODE_SELECTOR, exception_mem_page_fault);
	  for (c = 0; !got_pagefault; c += PAGE_SIZE) {
	  f |= *((char*)c);
	  }
	  
	  printk("Memory: 0x%x\n", c);
	*/
	printk("Testing interrupt hooking...");
	testinthook();
	
	printk("Hooking all exceptions...");
	exception_init();
	
	printk("IRQs...");
	hookint(0x20, KERNEL_CODE_SELECTOR, int_timer);
	hookint(0x21, KERNEL_CODE_SELECTOR, int_keyb);
	
	printk("software ints\n");
	hookint(0x81, KERNEL_CODE_SELECTOR, int_syscall);
	
	timer_init();
	
	settss(0x11, (void*)(PHYSICAL_LINEAR_ADDRESS + 0x20000));
	settss(0x12, (void*)(PHYSICAL_LINEAR_ADDRESS + 0x21000));
	
	//  for(;;)hlt();
	/*
	  asm(
	  #if 1
	  ".byte 0x9a\n\t"
	  #else
	  ".byte 0xea\n\t"
	  #endif
	  ".long 0\n\t"
	  ".word 0x90");
	  
	  for(;;)hlt();*/
	//  printk("Enabling interrupts...\n");
	//  sti();
}
