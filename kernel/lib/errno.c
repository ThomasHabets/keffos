/* $Id: errno.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/errno.h>
#include <keffos/stdlib.h>

int errno = 0; // FIXME: replace with define to ptr in current process

const char *strerror(int err)
{
	switch (abs(err)) {
	case EPERM:
		return "Operation not permitted";
	case ENOENT:
		return "No such file or directory";
	case EMAGIC:
		return "Magic number incorrect";
	case EFIXME:
		return "This error does not return correct error :-/";
	case ENOSYS:
		return "Function not implemented";
	default:
		return "Unknown error";
	}
}
