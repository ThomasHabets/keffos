/* $Id: ipc.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * IPC handling
 *
 */
#include <keffos/config.h>
#include <keffos/ipc.h>

/*
 * recieve returns a pointer to the first unhandled ipc
 * of type 'type' (can be "any") with timeout in millisecs.
 * timeout == -1 means infinite time.
 */
int recieve(pid_t from, int type, ipc_t *ret, int timeout)
{
	// return ptr to matching message
	return -(errno = ENOSYS);
}

/*
 * It can stall because queue could be full.
 * timeout == -1 means infinite time.
 */
int send(pid_t to, int type, ipc_t *msg, int timeout)
{
	// add msg to queue, return -EFIXME if timed out
	return -(errno = ENOSYS);
}
