/* $Id: console.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * consoel code for ia32
 * IOW: just a printer to 0xc00b8000.
 *
 */
#include <keffos/config.h>
#include <keffos/misc.h>
#include <keffos/console.h>
#include <asm/misc.h>
#include <asm/io.h>
#include <asm/misc.h>
#include <asm/string.h>
#include <sys/asm.h>
#include <keffos/string.h>

void console_init(void)
{
	text_width = 80;
	text_Bpp = 2;
	text_height = 50;
	
	// FIXME: Initial row should be read from, er... where we saved it.
	console_gotoxy(0,0xA);
}

void console_gotoxy(int x, int y)
{
	text_pos = y * text_width + x;
	//  console_set_cursor();
}

void console_set_cursor(void)
{
	int flags;
	
	// FIXME: just use a mutex for console ports
	save_flags(flags);
	cli();
	
	outb(15, 0x3d4);
	outb((text_pos & 0xff), 0x3d5);
	
	outb(14, 0x3d4);
	outb(text_pos >> 8, 0x3d5);
	
	restore_flags(flags);
}

void console_print(const char *msg, int len)
{
	int c;
	
	for (c = 0; c < len; c++) {
		switch (msg[c]) {
		case '\n':
			text_pos += text_width - (text_pos % text_width);
			break;
		case '\t':
			text_pos += 8;
			break;
		default:
			__asm__ ("movb %%al, (%%ebx)"
				 :
				 : "a" (msg[c]),
				 "b" (text_pos*text_Bpp+SCREEN_LINEAR_ADDRESS)
				);
			text_pos++;
			break;
		}
		if ((text_pos / text_width) == text_height) {
			console_scroll(1);
		}
	}
	console_set_cursor();
}

void console_setchar(int pos, char ch)
{
	pos *= text_Bpp;
	__asm__ ("movb %%al, (%%ebx)"
		 :
		 : "a" (ch),"b" (pos + SCREEN_LINEAR_ADDRESS)
		);
}

char console_getchar(int pos)
{
	char ret;
	pos *= text_Bpp;
	__asm__ ("movb (%%ebx), %%al"
		 : "=a" (ret)
		 : "b" (pos + SCREEN_LINEAR_ADDRESS)
		);
	return ret;
}

/*
 * FIXME SPEED: this is so damn slow, do it with HW scroll
 */
void console_scroll(int lines)
{
	int c;
	for (c = lines; c; c--) {
		memcpy(SCREEN_LINEAR_ADDRESS,
		       SCREEN_LINEAR_ADDRESS + text_width * text_Bpp,
		       text_width * text_Bpp * text_height);
		memsetw(SCREEN_LINEAR_ADDRESS+text_width*text_Bpp*text_height,
			0x0720,
			text_width * lines);
		text_pos -= text_width;
	}
}

void console_clear(void)
{
	memsetw(SCREEN_LINEAR_ADDRESS, 0x0720,
		(text_width * text_height * text_Bpp) / 2);
	text_pos = 0;
}
