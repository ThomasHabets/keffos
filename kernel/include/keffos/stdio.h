/* $Id: stdio.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_STDIO_H__
#define __INCLUDE_STDIO_H__

#define NULL ((void*)0)      /* FIXME: this is prolly the wrong place for this */

int printk(const char *fmt, ...);
int printd(int level, const char *fmt, ...);
int snprintf(char *ret, int n, const char *fmt, ...);

#endif
