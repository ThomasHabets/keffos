/* $Id: string.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_STRING_H__
#define __INCLUDE_STRING_H__

#include <sys/defines.h>
#include <keffos/limits.h>

int strnlen(const char *str, int len);
int strncmp(const char *str1, const char *str2, int maxlen);
char *strdup(const char *s);

#if !COMPILER_HAS_MEMCPY
void *memcpy(void *dest, const void *src, size_t n);
#endif

#if !COMPILER_HAS_MEMSET
void *memset(void *s, int c, int n);
void *memsetw(void *s, int c, int n);
#endif

#endif
