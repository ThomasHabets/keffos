/* $Id: symbols.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_SYMBOLS_H__
#define __INCLUDE_SYMBOLS_H__

#include <keffos/module.h>

typedef struct symbols_s {
  struct symbols_s *next;
  char *name;
  modules_t *module; /* NULL if in kernel */
  void *ptr;
  ptrsize_t size;
  int ref;           /* number of references, used by elf loader */
} symbols_t;

extern symbols_t kernel_symbols;

void loadsymbols(const char *data, int len);

#endif
