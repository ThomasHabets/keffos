/* $Id: stdio.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 */
#include <stdarg.h>
#include <keffos/config.h>
#include <keffos/string.h>
#include <keffos/misc.h>

int snprintf(char *ret, int len, const char *fmt, ...)
{
	va_list args;
	int i;
	
	va_start(args, fmt);
	i = vsnprintf(ret, len, fmt, args);
	va_end(args);
	
	return i;
}

int printk(const char *fmt, ...)
{
	va_list args;
	int i;
	char buf[1024];
	
	va_start(args, fmt);
	i = vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	
	console_print(buf, strnlen(buf, 1024));
	return i;
}


int printd(int level, const char *fmt, ...)
{
	va_list args;
	int i;
	char buf[1024];
	
	va_start(args, fmt);
	i = vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	
	printk(buf);
	return i;
}
