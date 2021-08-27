/* $Id: string.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * String routines
 * FIXME: All of these function probably have off-by-one errors
 *
 */
#include <keffos/config.h>
#include <keffos/string.h>
#include <keffos/limits.h>

char *strchr(const char *s, int c)
{
	for (;*s;s++) {
		if (*s == c) {
			return s;
		}
	}
	return NULL;
}

int strlen(const char *str)
{
	int c = -1;
	while (str[++c]) {
		;
	}
	return c;
}

// FIXME: is this stupid? (strNlen I mean)
// hmm, prolly
int strnlen(const char *str, int maxlen)
{
	int c = -1;
	while (str[++c]) {
		if (c > maxlen) {
			return maxlen;
		}
	}
	return c;
}

int strncmp(const char *str1, const char *str2, int maxlen)
{
	int c = -1;
	while (++c < maxlen) {
		if (str1[c] != str2[c]) {
			if (str1[c] < str2[c]) {
				return -1;
			} else {
				return 1;
			}
		} /* if !equal */
		if (!str1[c]) {
			return 0;
		}
	} /* while */
	return 0;
}

char *strndup(const char *s, int maxlen)
{
	char *ret;
	if (!(ret = (char*)kmalloc(strnlen(s, maxlen)+1))) {
		return NULL;
	}
	strncpy(ret, s, maxlen);
	return ret;
}


int strncpy(char *dest, const char *src, int maxlen)
{
	for (;*src && maxlen; maxlen--) {
		*dest = *src;
		*dest++;
		*src++;
	}
	*dest = 0;
}

#if !COMPILER_HAS_MEMCPY
void *memcpy(void *dest, const void *src, size_t n)
{
	int c;
	while (n--) {
		*((char*)dest++) = *((char*)src++);
	}
	return dest;
}
#endif /* !COMPILER_HAS_MEMCPY */

#if !COMPILER_HAS_MEMSET
void *memset(void *s, int c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		((char*)s)[i] = c;
	}
	return s;
}
#endif
#if !COMPILER_HAS_MEMSETW
void *memsetw(void *s, int c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		((__uint16*)s)[i] = c;
	}
	return s;
}
#endif
