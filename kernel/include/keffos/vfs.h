/* $Id: vfs.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_VFS_H__
#define __INCLUDE_VFS_H__

#include <keffos/limits.h>
#include <keffos/mutex.h>
#include <keffos/blockdev.h>
#include <sys/defines.h>

/*
 * data entity
 */
typedef struct inode_s {
	int type;   /* file, directory, fifo, character device, block device */
	int mode;   /* permission */
	uid_t uid;  /* owner */
	gid_t gid;  /* group */
	filesize_t size;     /* file size */
	char levels;         /* levels of pointers */
#define INODE_COMPRESS 0x01  /* data blocks are compressed */
#define INODE_ENCRYPT  0x02  /* data & inode are encrypted (only flags is unencrypted) */
#define INODE_SDELETE  0x04  /* wipe datablocks when deleted */
#define INODE_SYNC     0x08  /* sync after all writes */
#define INODE_UNDELETE 0x10  /* flag 'don't overwrite' on data blocks & pointers */
#define INODE_ROOT     0x20  /* this is the root directory */
	__uint32 flags;
	int links_count;           /* files pointing to this inode */
	__uint32 blocks;     /* blocks in use (including pointers) */
	time_t atime;        /* last access */
	time_t btime;        /* birth */
	time_t ctime;        /* inode change */
	time_t mtime;        /* data change */
	time_t dtime;        /* deleted */
} inode_t;

/*
 * cached inode
 */
typedef struct dentry_s {
  __uint32 magic;
  char *path;                   /* absolute path, no mountpoint */
  
  block_device_t *block_device;
  int dev;

  inode_t *inode;
  time_t last_used;
  char in_use;
} dentry_t;

/*
 * open file info
 */
typedef struct open_file_s {
  __uint32 magic;
  __uint64 pos;
  char mode;   /* read, read/write, write */
  dentry_t *dentry;
} open_file_t;


typedef struct fs_driver_s {
  __uint32 magic;
  char *name;               // "vfat", "vfat32", "ext2", "ffs", "keffs"
  char (*detect)  (block_device_t *device, int dev);

  /*
   * R/O operations
   */
  /* path is relative to device root */
  inode_t *(*stat)(block_device_t *device, int dev, const char *path);

  int (*readdir)  (block_device_t *device, int dev, inode_t *inode, inode_t **inodes, char ***filenames);
  int (*read)     (block_device_t *device, int dev, inode_t *inode, int offset, char *buf, int len);

  /*
   * create:
   *  mode:
   *   normal unix modes
   *  flags:
   *   used with special files (fifos n stuff)
   */
  int (*create)  (block_device_t *device, int dev, const char *path, int mode, int flags);
  int (*unlink)  (block_device_t *device, int dev, const char *path);
  int (*write)   (block_device_t *device, int dev, inode_t *inode, int offset, const char *buf, int len);
  
  int (*link)    (block_device_t *device, int dev, inode_t *inode, const char *src);

  /*
   * this is the *only* time when path is not relatice to
   * device (for src). This is pretty obvious when you think
   * about it
   */
  int (*symlink)    (block_device_t *device, int dev, const char *dest, const char *src);

  int (*mkdir)      (block_device_t *device, int dev, const char *path, int mode);
  int (*rename)     (block_device_t *device, int dev, const char *path, const char *newname);

  int (*chown)      (block_device_t *device, int dev, inode_t *inode, uid_t uid, gid_t gid);
  int (*chmod)      (block_device_t *device, int dev, inode_t *inode, int mode);
  
  int (*truncate)   (block_device_t *device, int dev, inode_t *inode, int size);
  mutex_t mutex;
  int in_use;              // if 0, it can be unloaded
  struct fs_driver_s *next;
} fs_driver_t;

int mount(const char *path, const char *device);
int mount2(const char *path, block_device_t *device, int dev);
int mount3(const char *path, block_device_t *device, int dev, fs_driver_t *handler);
int umount(const char *path);

#endif
