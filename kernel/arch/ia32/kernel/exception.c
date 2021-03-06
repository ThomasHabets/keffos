/* $Id: exception.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * C part of exception handler
 *
 */
#include <keffos/config.h>
#include <sys/exception.h>
#include <sys/asm.h>
#include <asm/misc.h>

char *exception_name[0x20] = {
	"divide error",
	"debug exception",
	"non-maskable interrupt",
	"breakpoint",
	"overflow",
	"bounds check",
	"invalid opcode",
	"coprocessor not available",
	"double fault",
	"(reserved 0)",
	"invalid task state segment",
	"segment not present",
	"stack exception",
	"general protection fault",
	"page fault",
	"(reserved 1)",
	"coprocessor error",
	"(reserved 2)",
	"(reserved 3)",
	"(reserved 4)",
	"(reserved 5)",
	"(reserved 6)",
	"(reserved 7)",
	"(reserved 8)",
	"(reserved 9)",
	"(reserved a)",
	"(reserved b)",
	"(reserved c)",
	"(reserved d)",
	"(reserved e)",
	"(reserved f)"
	"(reserved 10)",
};

int default_c_exception_handler(int num, char *stack)
{
	extern void panic(const char *msg);
	
	printk("Got exception: %s\n", exception_name[num]);
	switch (num) {
	case 0x0:
		panic("Unexpected exception");
		break;
	case 0x3:
		/*
		  this turned out to be wrong
		  ((int)stack[0])++;
		  printk("fixing return address\n");
		*/
		printk("Skipping breakpoint\n");
		break;
	default:
		panic("Unhandled exception");
	}
}

void exception_init(void)
{
	extern  exception0(),
		exception1(),
		exception2(),
		exception3(),
		exception4(),
		exception5(),
		exception6(),
		exception7(),
		exception8(),
		exception9(),
		exceptiona(),
		exceptionb(),
		exceptionc(),
		exceptiond(),
		exceptione(),
		exceptionf();
	
	hookint(0x0, KERNEL_CODE_SELECTOR, exception0);
	hookint(0x1, KERNEL_CODE_SELECTOR, exception1);
	hookint(0x2, KERNEL_CODE_SELECTOR, exception2);
	hookint(0x3, KERNEL_CODE_SELECTOR, exception3);
	hookint(0x4, KERNEL_CODE_SELECTOR, exception4);
	hookint(0x5, KERNEL_CODE_SELECTOR, exception5);
	hookint(0x6, KERNEL_CODE_SELECTOR, exception6);
	hookint(0x7, KERNEL_CODE_SELECTOR, exception7);
	hookint(0x8, KERNEL_CODE_SELECTOR, exception8);
	hookint(0x9, KERNEL_CODE_SELECTOR, exception9);
	hookint(0xa, KERNEL_CODE_SELECTOR, exceptiona);
	hookint(0xb, KERNEL_CODE_SELECTOR, exceptionb);
	hookint(0xc, KERNEL_CODE_SELECTOR, exceptionc);
	hookint(0xd, KERNEL_CODE_SELECTOR, exceptiond);
	hookint(0xe, KERNEL_CODE_SELECTOR, exceptione);
	hookint(0xf, KERNEL_CODE_SELECTOR, exceptionf);
}
