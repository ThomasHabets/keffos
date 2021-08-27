/* $Id: ramdrive.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_RAMDRIVE_H__
#define __INCLUDE_RAMDRIVE_H__

#include <keffos/vfs.h>

#pragma pack(1)
typedef struct ramdrive_entry_s {
	//  __uint8 filename_length;
	// ? filename (not asciiz)
	__uint16 permissions;
	uid_t uid;
	gid_t gid;
	__uint32 size;
	__uint32 flags;
	// flags:
	//  don't load (for modules in initrd)
	// last file has filename_length == 0
} ramdrive_entry_t;
#pragma unpack

fs_driver_t *ramdrive_init(void);

#endif
