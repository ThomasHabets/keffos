#ifndef __INCLUDE_ASM_MISC_H__
#define __INCLUDE_ASM_MISC_H__

#define hlt() asm("hlt")
#define sti() asm("sti")
#define cli() asm("cli")
#define nop() asm("nop")

#define save_flags(flags) { \
  __asm__ ("pushfl\n\t"     \
	   "popl %%eax"     \
	   : "=a" (flags)   \
	   : );             \
}

#define restore_flags(flags) { \
  __asm__ ("pushl %%eax\n\t"   \
	   "popfl"             \
	   :                   \
	   : "a" (flags));     \
}

#endif
