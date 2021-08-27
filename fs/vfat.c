/* $Id: vfat.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * FAT 12/16/32 driver
 *
 */
#include <keffos/stdio.h>

#if 0 // wrong order of things
static fs_driver_t fs_vfat = {
	FS_DRIVER_MAGIC,
	"vfat",
	&vfat_detect,
	&vfat_create,
	&vfat_unlink,
	NULL,             /* link */
	NULL,             /* symlink */
	NULL,             /* chown */
	NULL              /* chmod */
};
#endif

/*
 *
 */
int init_module(int argc, char **argv)
{
	register_fs(&fs_vfat);
}

int exit_module(int force)
{
	unregister_fs(&fs_vfat);
}
