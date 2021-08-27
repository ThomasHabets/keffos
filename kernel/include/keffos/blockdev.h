/* $Id: blockdev.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __INCLUDE_BLOCKDEV_H__
#define __INCLUDE_BLOCKDEV_H__

#include <sys/defines.h>

#define BLOCKDEV_SECTOR_SIZE 512

/*
 * block device (aic7xxx, ide, floppy)
 */
typedef struct block_device_s {
	__uint32 magic;
	
	/* "ide", "aic7xxx" */
	char *name;
	
	/*
	 * reads/writes blocks (1k blocks) from device to buf.
	 * 'count' is in blocks
	 */
	__uint64 (*read)  (int device, __uint64 block, void *buf,
			   __uint64 count);
	__uint64 (*write) (int device, __uint64 block, void *buf,
			   __uint64 count);
	
	/*
	 * special stuff like eject, sleep, lock, etc...
	 */
	int (*ioctl) (int device, int request, ...);
	
	/* --- module touches nothing below this point --- */
	/* ---       for use by vfs system only        --- */
	
	/*
	 * count of places this blockdev is in use
	 */
	unsigned int in_use;
	struct block_device_s *next;
} block_device_t;

#endif
