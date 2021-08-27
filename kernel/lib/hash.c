/* $Id: hash.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * General hash table handlers
 *
 */
#include <keffos/config.h>
#include <keffos/errno.h>

#if 0 // we need the size

typedef struct hash_leaf_s {
	__uint32 key;
	void *value;
	struct hash_leaf_s next;
} hash_t;

typedef struct hash_s {
	int size;
	hash_leaf_t *leaf;
}  hash_t;


hash_t *hash_create(int size)
{
	hash_leaf_t *ret;
	int c;
	
	if (!(ret = (hash_t*)kmalloc(sizeof(hash_t)))) {
		return -(errno = ENOMEM);
	}
	if (!(ret->leaf = (hash_leaf_t*)kmalloc(sizeof(hash_leaf_t) * size))) {
		free(ret);
		return -(errno = ENOMEM);
	}
	
	for (c = 0; c < size; c++) {
		ret->leaf[c].next = (hash_leaf_t*)0;
	}
	return ret;
}

int hash_lookup(void *value, hash_t *hash, __uint32 key)
{
	__uint32 h;
	hash_leaf_t *p;
	
	h = key % hash->size;
	p = &hash->leaf[h];
	for (; p; p = p->next) {
		if (key == p->key) {
			*value = p->value;
			return 0;
		}
	}
	return -(errno = EFIXME);
}

#endif
