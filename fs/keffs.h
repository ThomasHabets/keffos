/* $Id: keffs.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 */
#ifndef __INCLUDE_FS_KEFFS_H__
#define __INCLUDE_FS_KEFFS_H__

#include <keffos/vfs.h>

#define KEFFS_IDLEN 6
#define KEFFS_IDSTR "KefFS\0"

#define KEFFS_LABEL_MAXSIZE 32
#define KEFFS_LASTMOUNT_MAXSIZE 128
#define KEFFS_CREATOR_OS_MAXSIZE 16

#define KEFFS_FLAG_DIRTY      0x01
#define KEFFS_FLAG_JOURNALING 0x02
#define KEFFS_FLAG_UNDELETE   0x04

#define KEFFS_TYPE_DIR  1
#define KEFFS_TYPE_FILE 2
#define KEFFS_TYPE_FIFO 3

/*
 * max 512 bytes in total
 */
#pragma pack(1)
typedef struct keffs_superblock_s {
	char idstr[KEFFS_IDLEN];
	__uint16 revision;
	__uint64 id;
	__uint16 blocksize;
	__uint64 blocks;
	__uint64 freeblocks;
	__uint64 filecount;
	__uint64 inodecount;
	char label[KEFFS_LABEL_MAXSIZE];
	char last_mountpoint[KEFFS_LASTMOUNT_MAXSIZE];
	char creator_os[KEFFS_CREATOR_OS_MAXSIZE];
	__uint64 btime; /* created */
	__uint64 atime; /* last mount */
	__uint64 ctime; /* superblock change */ 
	__uint64 mtime; /* data change */
	__uint64 lastchecktime; /* last checked */
	__uint64 maxchecktime; /* time after last check to force check */
	__uint8 mountcount;
	__uint8 maxmountcount; /* count before check forced */
	__uint64 reserved_blocks;
	__uint8 error;   /* continue, mount-ro, panic */
	__uint32 flags; /* dirty, journaling, undelete */
	__uint64 root; /* root inode block */
	char reserved[215];
} keffs_superblock_t;
#pragma unpack

/*
 * disk inode, this is where file limits are defined
 */
#pragma pack(1)
typedef struct keffs_inode_s {
	__uint8 type;
	__uint16 mode;
	__uint32 uid;       /* this is not a runtime value, hence not uid_t */
	__uint32 gid;       /* same as above */
	__uint64 size;
	__uint8 levels;
	__uint32 flags;
	__uint8 links_count;
	__uint64 atime; /* access time */
	__uint64 btime; /* birth time */
	__uint64 ctime; /* inode change */
	__uint64 mtime; /* data change */
	__uint64 dtime; /* delete time */
} keffs_inode_t;
#pragma unpack

#pragma pack(1)
typedef struct keffs_direntry_s {
	__uint64 block;
	__uint8 name_len;
	/* char *name[name_len]; */
} keffs_direntry_t;
#pragma unpack

#endif
