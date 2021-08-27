// $Id: console.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
#include <config.h>

void console_init(void)
{
}

void console_print(char *msg, int len)
{
#asm
  mov ax, msg
#endasm
}
