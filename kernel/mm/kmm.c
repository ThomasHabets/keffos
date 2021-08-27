/* $Id: kmm.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * Keffos
 *
 * kernel memory manager
 *
 * callable from rest of kernel:
 * km_init();
 * kmalloc();
 * kfree();
 *
 * Thomas Habets       1999-08-09       Initial coding
 *
 * TODO:
 *   store pointers to blocks which are used (KM_STATE_USED)
 *   in a linked list to be able to free(addr) without knowing size.
 */
#include <keffos/config.h>
#include <keffos/mutex.h>
#include <keffos/klog.h>
#include <keffos/limits.h>

#define DEBUG 0

/*
 * FIXME: move these to a good place
 */
#define KM_HASHTABLE_SIZE 128

#define KM_BLOCK_TTL 300   /* in seconds */
#define KM_STATE_FREE 0
#define KM_STATE_USED 1

/*
 * Used to define a block of memory both for
 * allocated and unallocated blocks
 */ 
typedef struct km_block_s {
	void *ptr;
	ptrsize_t size;
	char state;
	int ttl;                  /* time to live when KM_STATE_FREE */
	struct km_block_s *next;
} km_block_t;

/*
 * This is the memory pool which allocated
 * blocks are in.
 */
mutex_t km_hashtable_mutex;
km_block_t km_hashtable[KM_HASHTABLE_SIZE]; // first block is blah.next

/*
 * Using km_block_t for free blocks actually wastes
 * a few byte per block ('state', 'ttl'). Just FYI.
 *
 * km_free_blocks must never get fragmented.
 */
mutex_t km_free_blocks_mutex;
int km_unallocated_block_count = 0;  /* for statistics */
km_block_t km_unallocated_blocks;    /* first block is blah.next */

/*
 * init km_free_blocks and km_hashtable[]
 */
void km_init(void)
{
	int c;
	km_block_t *cur;
	static km_block_t block1;       /* for mem below 0xA0000 */
	static km_block_t block2;       /* for mem above 1MB     */
	static char already_inited = 0;
	
	/*
	 * no need for mutex since multitasking 
	 * isn't enabled yet.
	 */
	printk("Initing kernel memory manager\n");
	if (already_inited) {
		panic("km_init() called twice");
	}
	already_inited = 1;
	
	for (c = 0; c < KM_HASHTABLE_SIZE; c++) {
		km_hashtable[c].next = NULL;
	}
	
	/*
	 * FIXME: use kernel size here,
	 * now it just uses 0xA0000 - 0x90000 = 0x10000 (64k)
	 */
	km_unallocated_block_count++;
	km_unallocated_blocks.next = &block1;
	
	/*
	 * FIXME: this assumes all mem < 0xA0000 is usable
	 */
	block1.ptr = (void*)0xc0090000;
	block1.size = 0xc00A0000 - 0xc0090000;
	block1.state = KM_STATE_FREE;    /* the only state valid */
	block1.next = &block2;
	
#if 1 /* FIXME: mem above 1MB is not detected yet */
	block1.next = NULL;
#else
	km_free_block_count++;
	block2.ptr = 0x100000;
	block2.size = top_of_mem - 0x100000;
	block2.state = KM_STATE_FREE;
	block2.next = NULL;
#endif
}

/*
 * Check allocated blocks for one that fits size
 * return 0 if no allocated block matched.
 */ 
void *km_getblock(size_t size)
{
	km_block_t *cur;
#if DEBUG
	printk("km_getblock(size = %d)\n", size);
#endif
	
	wait(0, km_hashtable_mutex);
	
	cur = km_hashtable[size % KM_HASHTABLE_SIZE].next;
	for (; cur; cur = cur->next) {
		if (cur->state == KM_STATE_FREE) {
			cur->state = KM_STATE_USED;
			signal(km_hashtable_mutex);
			return cur->ptr;
		}
	}
	
	signal(km_hashtable_mutex);
	return NULL;
}

/*
 * size is including the km_block_t that will store info about
 * the memory block itself.
 */
void *km_allocate(km_block_t *prev_block, int size)
{
	km_block_t *cur; /* allocated block */
#if DEBUG
	printk("\nkm_allocate(prev_block->next->ptr = %x, size = %d)\n", prev_block->next->ptr,size);
#endif
	cur = &km_hashtable[size % KM_HASHTABLE_SIZE];
	
	while(cur->next) {
		cur = cur->next;
	}
	
	cur->next = (km_block_t*)prev_block->next->ptr;
	cur = cur->next;
	cur->ptr = (km_block_t*)(prev_block->next->ptr + sizeof(km_block_t));
	
	cur->size = size;
	cur->state = KM_STATE_USED;
	cur->next = NULL;
	
	prev_block->next->size -= size + sizeof(km_block_t);
	prev_block->next->ptr += size + sizeof(km_block_t);
	
	if (!prev_block->next->size) {
		prev_block->next = prev_block->next->next;
	}
	
	return cur->ptr;
}

/*
 * Look for mem in this order:
 * 1) memory cache
 * 2) allocated block (may break block sizes)
 * 3) try to merge linear cached blocks
 * else fail
 */
void *kmalloc(size_t size)
{
	void *ret;
	km_block_t *cur;
#if DEBUG
	printk("kmalloc(size = %d)\n", size);
#endif
	/*
	 * unexpected behavior otherwise, and no other
	 * allocating function will be called.
	 */
	if (!size) {
		return NULL;
	}
	size = (size+3)&~3;
	
	if (!(ret = km_getblock(size))) {
		for (cur = &km_unallocated_blocks; cur->next; cur = cur->next) {
			if ((ptrsize_t)cur->next->size >= (ptrsize_t)(size + sizeof(km_block_t))) {
				return km_allocate(cur, size + sizeof(km_block_t));
			}
		}
		return NULL;
	}
	return ret;
}

void kfree2(void *ptr, size_t size)
{
	int c;
	km_block_t *p;
	wait(0, km_hashtable_mutex);
	
	for (p = &km_hashtable[c]; p->next; p = p->next) {
		if (ptr == p->ptr) {
			p->state = KM_STATE_FREE;
			p->ttl = KM_BLOCK_TTL;
			return;
		}
	}
	klog(LOG_WARNING, "kfree()", "block not found");
	signal(km_hashtable_mutex);
}

void unallocateblock(km_block_t *prev)
{
	klog(LOG_WARNING, "freeblock()", "not done");
}

void kfree(void *ptr)
{
	km_block_t *p;
	int c;
	
	if (!ptr) {
		return;
	}
	
	wait(0, km_hashtable_mutex);
	
	for (c = 0; c < KM_HASHTABLE_SIZE; c++) {
		for (p = &km_hashtable[c]; p; p = p->next) {
			if (ptr == p->ptr) {
				p->state = KM_STATE_FREE;
				p->ttl = KM_BLOCK_TTL;
				return;
			}
		}
	}
	
	klog(LOG_WARNING, "kfree()", "Block not found");
	
	signal(km_hashtable_mutex);
}
