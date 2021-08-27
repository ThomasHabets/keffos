/* $Id: vsnprintf.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * vsnprintf()
 *
 */
#include <stdarg.h>
#include <keffos/config.h>

#define is_digit(c)     ((c) >= '0' && (c) <= '9')

/*
 * FIXME: not really vsNprintf()
 * fix ASAP
 */
int vsnprintf(char *dest, int n, const char *src, va_list args)
{
	char *digitsl = "0123456789abcdef";
	char *digitsu = "0123456789ABCDEF";
	char *digits, buf[50], *str;
	unsigned int i, pad, zeropad, tmp, pos, base, ucase;
	char *orig;
	
	orig = dest;
	
	while (*src) {
		base = 10;
		pad =   zeropad =
			ucase = 
			pos = 0;
		
		if (*src == '%') {
			if(*++src == '.') {
				*src++;
				zeropad = 1;
			}
			if(is_digit(*src)) {
				pad = *src - '0';
				*src++;
			}
			switch(*src++) {
			case '%':
				*str++ = '%';
				break;
			case 's':
				if (!(str = va_arg(args, char *))) {
					str = "(null)";
				}
				for (;*str;) {
					*dest++ = *str++;
				}
				break;
			case 'c':
				*dest++ = (char)va_arg(args, char);
				break;
			case 'X':
				ucase = 1;
			case 'x':
				base = 16;
			case 'd':
			case 'i':
				tmp = va_arg(args, int);
				digits = (ucase) ? digitsu : digitsl;
				if (tmp < 0) {
					buf[pos++] = '-';
				}
				do {
					buf[pos++] = digits[tmp % base];
					tmp /= base;
				} while(tmp);
				if (pos < pad) {
					for (; pad-- > pos;) {
						*dest++ = (zeropad) ? '0' :' ';
					}
				}
				while (pos--) {
					*dest++ = buf[pos];
				}
				break;
			default:
				break;
			}
		} else {
			*dest++ = *src++;
		}
	}
	*dest = 0;
	return (int)(dest - orig);
}
