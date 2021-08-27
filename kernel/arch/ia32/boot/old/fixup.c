#include <keffos/config.h>
#include <elf.h>
#include <asm/misc.h>


extern void *fixupend;
Elf32_Ehdr *elfhdr;

void panic(char parm)
{
  __asm__ ("movb %%al, %%gs:(%%ebx)"
	   :
	   : "a" ('+'),"b" (0)
	   );
  __asm__ ("movb %%al, %%gs:(%%ebx)"
	   :
	   : "a" (parm),"b" (2)
	   );
  for (;;) {
    asm("hlt");
  }
}

void verify_elf_header(void)
{
  if (elfhdr->type != 2
      || elfhdr->version != 1
      || elfhdr->ehsize != 52
      || elfhdr->machine != 3) {
    panic('h');
  }
}

void fixup(void)
{
  int pos;
  elfhdr = (Elf32_Ehdr*)&fixupend;

  __asm__ ("movb %%al, %%gs:(%%ebx)"
	   :
	   : "a" ('E'),"b" (0)
	   );
  verify_elf_header();

  pos = 128;  /* program header */
  
  if (((char*)(elfhdr))[pos] != 'K') {
    panic('s');
  }

#if 0
  for (;;) {
    asm("hlt");
  }
#endif
}
