/* $Id: floppy.c,v 1.2 2001/05/31 21:32:20 marvin Exp $
 *
 * KeffOS
 *
 * Floppy driver
 *
 */
#include <keffos/config.h>
#include <keffos/stdio.h>

static int floppycount = 0;

/*
 *
 */
int init_module(int argc, char **argv)
{
	if (0) {
		floppycount++;
	}
	return (floppycount) ? 0 : -1;
}

/*
 *
 */
int exit_module(int force)
{
  
}
