/* $Id: ctype.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/config.h>

int tolower(int c)
{
	if (c < 'a') {
		return c + ('a' - 'A');
	}
	return c;
}
