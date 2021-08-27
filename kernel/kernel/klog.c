/* $Id: klog.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 */
#include <stdarg.h>
#include <keffos/config.h>
#include <keffos/klog.h>
#include <keffos/stdio.h>

/*
 * return: the number of chars written
 */
int klog(int type, const char *proc, const char *fmt, ...)
{
	va_list args;
	int i;
	char buf[1024];
	
	va_start(args, fmt);
	i = vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	
	return printk("%s: %s\n", proc, buf);
}
