/* $Id: mm.old.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * Keffos
 *
 * memory manager
 *
 * callable from rest of kernel:
 * mm_init();
 * malloc();
 * kfree();
 *
 * TODO:
 *   store pointers to blocks which are used (MM_STATE_USED)
 *   in a linked list to be able to free(addr) without knowing size.
 */
#include <keffos/config.h>
#include <keffos/mutex.h>
#include <keffos/klog.h>

/*
 * FIXME: move these to a good place
 */
#define MM_HASHTABLE_SIZE 128
#define size_t int
#define NULL 0

#define MM_BLOCK_TTL 300   /* in seconds */
#define MM_STATE_FREE 0
#define MM_STATE_USED 1

/*
 * Used to define a block of memory both for
 * allocated and unallocated blocks
 */ 
typedef struct mm_block_s {
	void *ptr;
	size_t size;
	char state;
	int ttl;                  /* time to live when MM_STATE_FREE */
	struct mm_block_s *next;
} mm_block_t;

/*
 * This is the memory pool which allocated
 * blocks are in.
 */
mutex_t mm_hashtable_mutex;
mm_block_t mm_hashtable[MM_HASHTABLE_SIZE]; // first block is blah.next

/*
 * Using mm_block_t for free blocks actually wastes
 * a few byte per block ('state', 'ttl'). Just FYI.
 *
 * mm_free_blocks must never get fragmented.
 */
mutex_t mm_free_blocks_mutex;
int mm_unallocated_block_count = 0;  /* for statistics */
mm_block_t mm_unallocated_blocks;    /* first block is blah.next */

/*
 * init mm_free_blocks and mm_hashtable[]
 */
void mm_init(void)
{
	int c;
	mm_block_t *cur;
	static mm_block_t block1;       /* for mem below 0xA0000 */
	static mm_block_t block2;       /* for mem above 1MB     */
	static char already_inited = 0;

	/*
	 * no need for mutex since multitasking 
	 * isn't enabled yet.
	 */

	if (already_inited) {
		panic("mm_init() called twice");
	}
	already_inited = 1;

	for (c = 0; c < MM_HASHTABLE_SIZE; c++) {
		mm_hashtable[c].next = NULL;
	}

	/*
	 * FIXME: use kernel size here,
	 * now it just uses 0xA0000 - 0x90000 = 0x10000 (64k)
	 */
	mm_unallocated_block_count++;
	mm_unallocated_blocks.next = &block1;

	/*
	 * FIXME: this assumes all mem < 0xA0000 is usable
	 */
	block1.ptr = (void*)0xc0090000;
	block1.size = 0xc00A0000 - 0xc0090000;
	block1.state = MM_STATE_FREE;    /* the only state valid */
	block1.next = &block2;

#if 1 /* FIXME: mem above 1MB is not detected yet */
	block1.next = NULL;
#else
	mm_free_block_count++;
	block2.ptr = 0x100000;
	block2.size = top_of_mem - 0x100000;
	block2.state = MM_STATE_FREE;
	block2.next = NULL;
#endif
}

/*
 * Check allocated blocks for one that fits size
 * return 0 if no allocated block matched.
 */ 
void *mm_getblock(size_t size)
{
	mm_block_t *cur;

	wait(0, mm_hashtable_mutex);

	cur = mm_hashtable[size % MM_HASHTABLE_SIZE].next;
	for (; cur; cur = cur->next) {
		if (cur->state == MM_STATE_FREE) {
			cur->state = MM_STATE_USED;
			signal(mm_hashtable_mutex);
			return cur->ptr;
		}
	}
  
	signal(mm_hashtable_mutex);
	return NULL;
}

/*
 * size is including the mm_block_t that will store info about
 * the memory block itself.
 */
void *mm_allocate(mm_block_t *prev_block, int size)
{
	mm_block_t *alloc_block; /* allocated block */

	alloc_block = &mm_hashtable[size % MM_HASHTABLE_SIZE];

	while(alloc_block->next) {
		alloc_block = alloc_block->next;
	}
	alloc_block->next = (mm_block_t*)prev_block->next->ptr;
	alloc_block = alloc_block->next;
	alloc_block->ptr = (mm_block_t*)prev_block->next->ptr + sizeof(mm_block_t);
	alloc_block->size = size;
	alloc_block->state = MM_STATE_USED;
	alloc_block->next = NULL;

	prev_block->next->size -= size;
	prev_block->next->ptr += size;
	if (!prev_block->next->size) {
		prev_block->next = prev_block->next->next;
	}

	return alloc_block->ptr;
}

/*
 * Look for mem in this order:
 * 1) memory cache
 * 2) allocated block (may break block sizes)
 * 3) try to merge linear cached blocks
 * else fail
 */
void *malloc(size_t size)
{
	void *ret;
	mm_block_t *cur;

	/*
	 * unexpected behavior otherwise, and no other
	 * allocating function will be called.
	 */
	if (!size) {
		return NULL;
	}

	if (!(ret = mm_getblock(size))) {
		for (cur = &mm_unallocated_blocks; cur->next; cur = cur->next) {
			if (cur->next->size >= (size + sizeof(mm_block_t))) {
				return mm_allocate(cur, size + sizeof(mm_block_t));
			}
		}
		return NULL;
	}
	return ret;
}

void kfree(void *ptr, size_t size)
{
	int c;
	mm_block_t *p;
	wait(0, mm_hashtable_mutex);
  
	for (p = &mm_hashtable[c]; p->next; p = p->next) {
		if (ptr == p->ptr) {
			p->state = MM_STATE_FREE;
			p->ttl = MM_BLOCK_TTL;
			return;
		}
	}
	klog(LOG_WARNING, "kfree()", "block not found");
	signal(mm_hashtable_mutex);
}

void unallocateblock(mm_block_t *prev)
{
	klog(LOG_WARNING, "freeblock()", "not done");
}

void free(void *ptr)
{
	mm_block_t *p;
	int c;
	wait(0, mm_hashtable_mutex);
  
	for (c = 0; c < MM_HASHTABLE_SIZE; c++) {
		for (p = &mm_hashtable[c]; p; p = p->next) {
			if (ptr == p->ptr) {
				p->state = MM_STATE_FREE;
				p->ttl = MM_BLOCK_TTL;
				return;
			}
		}
	}

	klog(LOG_WARNING, "free()", "Block not found");

	signal(mm_hashtable_mutex);
}
