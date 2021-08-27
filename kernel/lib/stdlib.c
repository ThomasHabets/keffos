/* $Id: stdlib.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/config.h>
#include <keffos/stdlib.h>

int abs(int j)
{
	return (( j > 0) ? j : -j);
}
