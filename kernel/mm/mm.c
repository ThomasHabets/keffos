/* $Id: mm.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * Keffos
 *
 * memory manager
 *
 * callable from rest of kernel:
 * mm_init();
 * mmap();
 * munmap();
 *
 * TODO:
 *   store pointers to blocks which are used (MM_STATE_USED)
 *   in a linked list to be able to free(addr) without knowing size.
 */
#include <keffos/config.h>
#include <keffos/mutex.h>
#include <keffos/klog.h>
#include <sys/defines.h>

#define PAGE_REA

/*
 * pages
 */
typedef struct page_s {
	struct page_s *next;
	__uint32 page;       /* only 22 bits needed for x86 */
	int where;
	__uint32 pageframe;  /* -----------||-------------- */
	__uint16 flags;
} page_t;

/*
 * First free memory block @ 0xc0090000
 */
void vm_init(void)
{
	km_init();
}

int mmap(__uint32 startpage, __uint32 pages)
{
	//  if (
}

void munmap(__uint32 pageframe)
{
  
}
