/* $Id: console.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

void console_print(const char *msg, int len);
void console_scroll(int lines);
void console_init(void);
void console_gotoxy(int x, int y);
void console_set_cursor(void);

#endif
