/* $Id: kdebug.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#include <keffos/config.h>
#include <sys/asm.h>
#include <sys/int.h>
#include <keffos/string.h>
#include <asm/misc.h>
#include <keffos/shutdown.h>

char ASCIINames[] = {
/*
 *       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
 */
	0,   27,'1','2','3','4','5','6','7','8','9','0','-','=',  8,  9,  // 0
	'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,'a','s',  // 1
	'd','f','g','h','j','k','l',';', 39,'`',  0, 92,'z','x','c','v',  // 2
	'b','n','m',',','.','/',  0,'*',  0,' ',  0,  0,  0,  0,  0,  0,  // 3
	0  ,  0,  0,  0,  0,  0,  0,'7','8','9','-','4','5','6','+','1',  // 4
	'2','3','0',127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 5
	0  ,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 6
	0  ,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 7
};

void kdebug_int_keyb(void);
void kdebug_int_timer(void);

volatile char *kdebug_kbbuf;
volatile char kdebug_kbdone;
volatile int kdebug_pos;

volatile char lastbyte = 0;

void kdebug_keybhandler(unsigned char data)
{
	if (kdebug_kbbuf) {
		if (data > 127) {
			return;
		}
		data = ASCIINames[data];
		kdebug_kbbuf[kdebug_pos++] = data;
		lastbyte = data;
		printk("%c", data);
	}
}

void kdebug_getstr(char *buf, int len)
{
	kdebug_kbbuf = buf;
	kdebug_kbdone = kdebug_pos = lastbyte = 0;
	sti();
	for (;;) {
		switch (lastbyte) {
		case '0':
			break;
		case '\n':
			goto done;
		}
	}
 done:;
	cli();
	kdebug_kbbuf[kdebug_pos - 1] = 0;
	kdebug_kbbuf = 0;
}

void kdebug_main(void)
{
#define CMDLEN 127
	char cmd[CMDLEN + 1];
	static kdebug_running = 0;
	
	if (kdebug_running) {
		printk("Bleh, kdebug called from within kdebug\nHalting...");
		cli();
		for(;;) hlt();
	}
	
	kdebug_running = 1;
	
	for(;;) {
		printk("kdebug> ");
		kdebug_getstr(cmd, CMDLEN);
		if (!cmd[0]) {
			continue;
		}
#if 0
		printk("Executing command: (%d) '%s'\n", strlen(cmd), cmd);
#endif
		if (!strncmp(cmd, "help", CMDLEN)) {
#if 0
			printk(""
"help/h                              Show this help text\n"
"clear                               Clear the screen\n"
"backtrace/bt  <addr>                Backtrace on the stack @ addr (not done)\n"
"call          <addr>                Call function @ addr (not done)\n"
"disassemble/u <addr>                Disassemble addr (not done)\n"
"printf        <fmt> <addr>          Print data (not done)\n"
"dump/d        <addr> <len>          Dump data @ addr (not done)\n"
"regs/r                              Dump regs (not done)\n"
"\n"
"continue/c                          Continue at panic pos (DANGEROUS!)\n"
"jump/j        <addr>                Continue @ addr (not done)\n"
"\n"
"lock/l                              Lock debugger with password (not done)\n"
"write/w       <dev> <addr> <size>   Write data to device (not done)\n"
"sym/s         <addr>                Lookup symbol (not done)\n"
"addr          <symbol>              Lookup address (not done)\n"
"reboot/b                            Reboot computer\n"
"l2p           <addr>                Translate linear addr to pages (not done)\n"
"p2l           <pde> <pte> <off>     Translate pages to linear addr (not done)\n"
"off/o                               Power down\n");
#endif
		} else if (!strncmp(cmd, "clear", CMDLEN)) {
			console_clear();
		} else if (!strncmp(cmd, "continue", CMDLEN)
			   || !strncmp(cmd, "c", CMDLEN)) {
			kdebug_running = 0;
			return;
		} else if (!strncmp(cmd, "reboot", CMDLEN)
			   || !strncmp(cmd, "b", CMDLEN)) {
			shutdown(SHUTDOWN_REBOOT);
		} else if (!strncmp(cmd, "off", CMDLEN)
			   || !strncmp(cmd, "o", CMDLEN)) {
			shutdown(SHUTDOWN_OFF);
		} else {
			printk("unknown command or command not done\n");
		}
	}
}

void kdebug(void)
{
	int *timer,*keyb;
	int noresume = 0;
	
	printk("Running kdebug 1.0\n");
	
	if (!(timer = save_inthook(0x20))) {
		printk("Unable to save timer int\n");
		noresume = 1;
	}
	if (!(keyb = save_inthook(0x21))) {
		printk("Unable to save keyb int\n");
		noresume = 1;
	}
	hookint(0x20, KERNEL_CODE_SELECTOR, kdebug_int_timer);
	hookint(0x21, KERNEL_CODE_SELECTOR, kdebug_int_keyb);
	
	if (noresume) {
		printk("Unable to save some ints\nCan't 'continue'\n");
	}
	do {
		kdebug_main();
	} while (noresume);

	restore_inthook(timer);
	restore_inthook(keyb);
	
	kfree(timer);
	kfree(keyb);
}
