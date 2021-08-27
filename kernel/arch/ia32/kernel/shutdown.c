/* $Id: shutdown.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * Halt/reboot/off
 *
 */
#include <keffos/config.h>
#include <keffos/shutdown.h>
#include <asm/misc.h>

void shutdown(int type)
{
	switch (type) {
	case SHUTDOWN_HALT:
		/*
		 * FIXME: change interrupt-handlers to ignore
		 */
		hlt();
		for(;;);
	case SHUTDOWN_REBOOT:
		/*
		 * Only reason this works is that panic() doesn't
		 * change ds before using it, and causes a double,
		 * and then tripple-fault.
		 */
		asm("pushw $0\n\t"
		    "popw %ds\n\t"
		    "movl $0, 0\n\t");
	case SHUTDOWN_OFF:
		panic("shutdown(SHUTDOWN_OFF) is not done yet");
	case SHUTDOWN_KDEBUG:
		kdebug();
	default:
		panic("shutdown(*unknown*) called");
	}
}
