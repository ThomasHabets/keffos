/* $Id: checksum.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * Checksum routines
 *
 * Thomas Habets         -           Initial coding
 *
 */
#include <keffos/config.h>
#include <sys/defines.h>

__uint16 checksum16(__uint16 *buf, int num)
{
	__uint32 sum;
	
	for (sum = 0; num > 0; num--) {
		sum += *buf++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (__uint16)~sum;
}

// FIXME: md5
