/* $Id: timer.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * C task switcher
 *
 */
#include <keffos/config.h>
#include <keffos/string.h>
#include <keffos/process.h>
#include <sys/asm.h>
#include <sys/defines.h>

#pragma pack(1)
typedef struct ia32_stack_reg_frame_s {
	__uint32 cr3, ebp, edi, esi, edx, ecx, ebx, eax;
	__uint16 gs,fs,ss,es,ds;
	__uint32 eip;
	__uint16 cs;
	__uint32 eflags;
} ia32_stack_reg_frame_t;
#pragma unpack

int processcount;
process_t processlist[10]; // just a static list, will be other
process_t nullprocess;
int curtask = 0;

process_t tasks[3];

void task0(void)
{
	__asm__("sti");
	for(;;) {
		__asm__("incb (0xc00b8002)");
		schedule();
	}
}

int create_free_process(void (*func)(void))
{
	ia32_stack_reg_frame_t *frame;
	static int c = 0;
	c++;
	
	if (!(tasks[c].stack = (__uint32) kmalloc(1024))) {
		panic("Not enough memory");
	}
	printk("tasks[%d].stack = 0x%.8x\n", c, tasks[c].stack);
	tasks[c].stack += 512;
	
	frame = (ia32_stack_reg_frame_t*)tasks[c].stack;
	
	frame->cr3 = 0x11000;
	frame->eax = 0;
	frame->ebx = 0;
	frame->ecx = 0;
	frame->edx = 0;
	
	frame->esi = 0;
	frame->edi = 0;
	frame->ebp = 0;
	
	frame->ss = KERNEL_STACK_SELECTOR;
	frame->ds = KERNEL_DATA_SELECTOR;
	frame->es = KERNEL_DATA_SELECTOR;
	frame->fs = KERNEL_DATA_SELECTOR;
	frame->gs = KERNEL_DATA_SELECTOR;
	frame->cs = KERNEL_CODE_SELECTOR;
	
	frame->eip = func;
	frame->eflags = 0x02020000;  
}

volatile int inited = 0;
void timer_init(void)
{
	create_free_process(task0);
	inited = 1;
}


ptrsize_t next_process(ptrsize_t stack)
{
	static int blah = 0;
	static unsigned int b;
	
	tasks[curtask].stack = stack;
	
	__asm__("incb (0xc00b8008)");
	if (inited) {
		__asm__("incb (0xc00b8000)");
		curtask = 1 - curtask;
	}
	//  printk("Stack @ 0x%.8x   ", tasks[curtask].stack);
	//  printk("Eflags: %.8x\n", ((ia32_stack_reg_frame_t*)tasks[curtask].stack)->eflags);
	//  ((ia32_stack_reg_frame_t*)tasks[curtask].stack)->eflags = 0xfc0001;
	stack = tasks[curtask].stack;
	return stack;
}
