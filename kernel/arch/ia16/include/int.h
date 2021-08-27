/* $Id: int.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * Int handling, but no inthandler prototypes.
 * Most of them should be in modules anyway.
 */
#ifndef __INCLUDE_SYS_INT_H__
#define __INCLUDE_SYS_INT_H__

#include <sys/defines.h>

/*
 * this is IMHO a good way to prototype asm functions which
 * will not be called by code
 */
extern void testint();
extern void int_keyb();
extern void int_timer();

void hookint(__uint8 num, __uint16 segment, void (*func)(void));
__uint32 *save_inthook(__uint8 num);
void restore_inthook(__uint32 *saved);

#endif

