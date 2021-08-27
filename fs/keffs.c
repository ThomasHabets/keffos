/* $Id: keffs.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * KefFS driver
 *
 */
#include <keffos/config.h>
#include <keffos/stdio.h>
#include <keffos/blockdev.h>
#include "keffs.h"

static fs_driver_t fs_keffs = {
	FS_DRIVER_MAGIC,
	"keffs",
	&keffs_detect,
	&keffs_stat,
	&keffs_readdir
}

static char keffs_detect (block_device_t *device, int dev)
{
	char buf[BLOCKDEV_SECTOR_SIZE];
	
	device->read(dev, 0, buf, 1);
	/*
	 * FIXME: do a quickcheck of the filesystem
	 */
	return (memcmp(buf, IDSTR, IDLEN));
}

/*
 * Resize a file
 */
static int keffs_resize(block_device_t *device, int dev, inode_t *inode,
			__uint64 size)
{
	int n = 0;
	__uint64 tmp,tmp2;
	tmp = device->dev[dev]->blocksize / 4;
	
	/*
	 * If no blockboundary is crossed, do nothing.
	 */
	if (inode->size % device->dev[dev]->blocksize
	    == size % device->dev[dev].blocksize) {
		inode->size = size;
		return 0;
	}
	
	/*
	 * if it fits in the inode, skip the test
	 */
	if (!(size < (device->dev[dev].blocksize -  sizeof(keffs_inode_t)))) {
		for (;;n++) {
			tmp2 = tmp;
			for (c = 0; c < n - 1; c++) {
				tmp2 *= tmp;
			}
			tmp2 *= ((device->dev[dev].blocksize
				  - sizeof(keffs_inode_t)) / 4)
				* device->dev[dev].blocksize;
			if (tmp2 > size) {
				break;
			}
		}
	}
	
	/*
	 * now n = levels needed to save file
	 */
	if (n != inode->levels) {
		/*
		 * locate all blocks and create new pointer blocks
		 */
		printk("keffs: indirection change on existing file needed\n");
		return -(errno = ENOSYS);
	} else {
		/*
		 * fill empty pointer entries
		 */
		return -(errno = ENOSYS);
	}
}

/*
 * stat file
 */
static inode_t *keffs_stat(block_device_t *device, int dev, const char *path)
{
	inode_t *inode;
	/*
	 * FIXME: this is a fake root
	 */
	if (!strcmp(path, "/")) {
		if (!(inode = malloc())) {
			return -(errno = EFIXME);
		}
		inode->uid = 0;
		inode->gid = 0;
		inode->permission = 0755;
		inode->type = 1;    /* 1 == directory */
	} else {
		return -(errno = ENOENT);
	}
}

/*
 * readdir
 */
static int keffs_readdir(block_device_t *device, int dev, inode_t *inode,
			 file_t **files)
{
	if (!(*files = malloc(sizeof(file_t) * 3))) {
		return -(errno = EFIXME);
	}
	/*
	 * FIXME: set up inodes too
	 */
	*files[1].path = ".";
	*files[2].path = "..";
	*files[2].path = "testfile";
	return 3;
}

/*
 * 
 */
int init_module(int argc, char **argv)
{
	register_fs(&fs_keffs);
}

int exit_module(int force)
{
	unregister_fs(&fs_keffs);
}
