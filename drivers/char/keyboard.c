/* $Id: keyboard.c,v 1.2 2001/05/31 21:32:20 marvin Exp $
 *
 * KeffOS
 *
 * Keyboard handler
 *
 */
#include <keffos/stdio.h>
#include <keffos/io.h>
#include <keffos/errno.h>

/*
 * 'data' was pushed by the asm-proc.
 * you can't push bytes
 *
 * This function is called in cli mode.
 * cli/sti is handled in asm, don't touch it here.
 *
 */
void c_int9_handler(__uint16 data)
{
}

/*
 *
 */
int init_module(int argc, char **argv)
{
	return 0;
}
