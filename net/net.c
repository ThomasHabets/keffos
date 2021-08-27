/* $Id: net.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * network device demux
 *
 */
#include <keffos/config.h>
#include <keffos/net.h>

net_protocol_struct_t *net_protocol_handler(enum protocol_e protocol);
{
	errno = -ENOSYS;
	return (void*)0;
}
