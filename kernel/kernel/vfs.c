/* $Id: vfs.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 */
#include <keffos/config.h>
#include <keffos/vfs.h>
#include <keffos/klog.h>
#include <keffos/errno.h>
#include <keffos/limits.h>
#include <keffos/magic.h>

/*
 *
 */
int mount(const char *path, const char *device)
{
	// FIXME: go through list of filesystems with driver->detect and
	// mount2() the right one
	return -(errno = ENOSYS);
}

int mount3(const char *path, block_device_t *device, int dev,
	   fs_driver_t *handler)
{
	wait(handler->mutex);
	if (handler->magic != FS_DRIVER_MAGIC) {
		klog(LOG_INTERNAL_ERROR, "mount()",
		     "Magic has changed since fs registered. magic should be 0x%x, is 0x%x", FS_DRIVER_MAGIC, handler->magic);
		signal(handler->mutex);
		return -(errno = EMAGIC);
	}
	
#if 1
	if (!handler->detect) {
		klog(LOG_STRUCT_ERROR, "mount()", "Module has no detect proc");
		signal(handler->mutex);
		return -(errno = EFIXME);
	}
	if (handler->detect(device, dev)) {
		klog(LOG_WARNING, "mount()", "fs_driver[%s]->detect() does not like you", handler->name);
		signal(handler->mutex);
		return -(errno = EBUSY);
	}
#else
	signal(handler->mutex);
	return -(errno = ENOSYS);
#endif
	handler->in_use++;
	signal(handler->mutex);
	return 0;
}

int umount(const char *path)
{
	return -(errno = ENOSYS);
}
