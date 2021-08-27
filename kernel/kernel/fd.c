/* $Id: fd.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/config.h>

typedef struct fd_functions_s {
	int (*read)(int fd, void *buf, size_t count);
	int (*write)(int fd, void *buf, size_t count);
	int sync(void);
} fd_functions_t;

typedef struct fd_s {
	int size, ipos, opos;
	int flags;
	
	char *buffer;
	
	fd_functions_t *funcs;
} fd_t;

int dev_zero_read(int fd, void *buf, int size)
{
	int c;
	for (c = 0; c < size; c++) {
		((char*)buf)[c] = 0;
	}
	return size;
}

int dev_zero_write(int fd, void *buf, int size)
{
	return size;
}

/*
 *
 */
int fd_create(unsigned int size, unsigned int flags, int type)
{
	fd_t *fd;
	if (!size) {
		size = 4096;
	}
	if (!(fd = (fd_t*)kmalloc(sizeof(fd_t)))) {
		return -(errno = ENOMEM);
	}
	if (!(fd->buffer = (char*)kmalloc(size))) {
		kfree(fd);
		return -(errno = ENOMEM);
	}
	fd->funcs = hash_lookup(&fd_funcs, type);
	fd->size = size;
	fd->ipos = 0;
	fd->opos = 0;
}

/*
 * push data to a fd
 */
int fd_write(int fd, const char *buf, size_t count)
{
}

/*
 * pop data from a fd
 */
size_t fd_read(int fd, void *buf, size_t count)
{
}

/*
 * Called when both fds are closed by all processes
 * Deallocs filedes_t struct
 */
int fd_destroy(int filedes[2])
{
	/*
	 * 0) check if both fds are closed
	 * 1) find filedes_t struct
	 * 2) filedes_t (on that list) *lock*
	 * 3) dealloc
	 * 4) return
	 */
	return -(errno = ENOSYS);
}
