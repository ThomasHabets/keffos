/* $Id: write.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#include <unistd.h>
#include <errno.h>
#include <keffos/symbols.h>

ssize_t write(int fd, const void *buf, size_t count)
{
	syscall_parameters_t p;
	int ret;
	
	p.p0 = (void*)buf;
	p.i64_0 = count;
	__asm__("int $0x81"
		: "=a" (ret)
		: "a" (SYS_write), "d" (&p));
	if (ret < 0) {
		errno = -ret;
		return -1;
	}
	return ret;
}
