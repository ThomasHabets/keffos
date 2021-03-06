/* $Id: main.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * C code entry point and small stuff 
 *
 */
#include <keffos/config.h>
#include <keffos/stdio.h>
#include <keffos/cpuid.h>
#include <keffos/misc.h>
#include <keffos/ramdrive.h>
#include <keffos/errno.h>
#include <keffos/setup.h>
#include <keffos/symbols.h>
#include <asm/misc.h>
#include <sys/asm.h>

fs_driver_t fs_ramdrive;
extern __uint32 kernel_size;
__int16 text_height = 0;
__int16 text_width = 0;
__int16 text_pos = 0;
__int16 text_Bpp = 0;
symbols_t kernel_symbols;

int enable_kdebug = 1;

int multitask = 0;

/*
 * this is just a small test thing, will be put in a module
 * as soon as modules work.
 */
#define KEYB_BUF_SIZE 256
int keyb_in_ptr = 0;        // next index to fill
int keyb_out_ptr = 0;       // next index to send to program
char keyb_buffer[KEYB_BUF_SIZE];

// untested code running in cli mode, called from int_keyb
void keybhandler(char data)
{
	printk("C Keybhandler says hello!\n");
	// FIXME: check for overflow
	keyb_buffer[keyb_in_ptr++ % KEYB_BUF_SIZE] = data;
	keyb_in_ptr %= KEYB_BUF_SIZE;
}

/*
 * Below is what is really supposed to be here
 */

/*
 * freezes the kernel
 * FIXME: needs asm rewrite: can't get (correct) esp or EIP otherwise
 */
void panic(const char *msg)
{
	__uint32 eax, ebx, ecx, edx, esi, edi, ebp, esp, eip;
	unsigned short cs,ds,es,ss,fs,gs;
	int flags;
	
	save_flags(flags);
	cli();
	
	eip = 0xffffffff;
	
	__asm__(""
		: "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx),
		"=S" (esi), "=D" (edi)
		:);
	__asm__("movl %%esp, %%eax\n\t" /* wrong esp */
		"movl %%ebp, %%ebx\n\t"
		: "=a" (esp), "=b" (ebp)
		:);
	__asm__("pushw %%cs; popw %%ax\n\t"
		"pushw %%ds; popw %%bx\n\t"
		"pushw %%es; popw %%cx\n\t"
		"pushw %%ss; popw %%dx\n\t"
		"pushw %%fs; popw %%si\n\t"
		"pushw %%gs; popw %%di\n\t"
		: "=a" (cs),
		"=b" (ds),
		"=c" (es),
		"=d" (ss),
		"=S" (fs),
		"=D" (gs)
		:);

// max printk msg is 1024 bytes, if long dash-line, some errors won't fit
	printk("\n--------------\n"
	       "KERNEL PANIC!!!!\n"
	       "Fucking shit! Sonofagoddamnbitch!\n"
	       "Details: %s\n"
	       "errno:   (0x%x) %s\n", msg, errno, strerror(errno));
	
	printk("EIP: 0x%.8x\n", eip);
	printk("CS: 0x%.4x  DS: 0x%.4x  ES: 0x%.4x    SS: 0x%.4x  FS: 0x%.4x  GS: 0x%.4x\n", cs, ds, es, ss, fs, gs);
	printk("EAX: 0x%.8x    EBX: 0x%.8x    ECX: 0x%.8x    EDX: 0x%.8x\n", eax, ebx, ecx, edx);
	printk("ESI: 0x%.8x    EDI: 0x%.8x    EBP: 0x%.8x    ESP: 0x%.8x\n", esi, edi, ebp, esp);
	
	if (enable_kdebug) {
		kdebug();
	} else {
		cli();
		printk("Kdebug disabled\n");
		printk("Halting...");
		for(;;) {
			hlt();
		}
		/*
		 * FIXME: time out for reboot (sysctl)
		 */
	}
	restore_flags(flags);
}

void testmalloc(void)
{
	char *test;
	int c;
#define SIZE 10
	
	printk("Testing malloc ");
	if (!(test = (char*)kmalloc(SIZE+1))) {
		panic("Coundn't allocate test mem");
	}
	printk("(0x%x)...", test);
	strncpy(test, "ok\n", SIZE);
	printk("%s", test);
	kfree(test);
}

/*
 * C code entry point
 */
void start_kernel(void)
{
	processor_info_t cpuid;
	char ch;
	
	cli();
	console_init();
	vm_init();
	
	printk("C rules the cosmos!\n");
	printk("KeffOS %d.%d.%d\n", VERSION, RELEASE, PATCHLEVEL);
	
	setup_arch();

	get_processor_info(&cpuid);
	printk("Professor:\n"
	       "\tVendor: %s\n"
	       "\tModel:  %s\n"
	       "\tClock:  %d Mhz\n", cpuid.vendor, cpuid.model, cpuid.speed);
	
	testmalloc();
	
	{
		unsigned int lin = 0xC0000000;
		int a,b,c;
		linear_to_page(&a,&b,&c, lin);
		printk("0x%x = 0x%.3x:0x%.3x:0x%.3x", lin, a, b, c);
		lin = page_to_linear(a,b,c);
		printk(" = 0x%x\n", lin);
	}
	
	//  asm(".byte 0xcc");
	
	//  ramdrive_loadall();
	
	printk("filesystem %s...\n", "/dev/whatever");
	if (0 > mount("/", "/dev/whatever")) {
		//    panic("Unable to mount root filesystem");
	}
	
	printk("usermode\n");
	sti();
	for(;;) {
		__asm__("incb (0xc00b8004)");
	} 
	init();
	panic("Init returned");
	for(;;);
	
	return;
}
