/* $Id: ide.c,v 1.2 2001/05/31 21:32:20 marvin Exp $
 *
 * KeffOS
 *
 * Normal (E)IDE driver
 *
 */
/*
 * NOTE:
 * Logical_sector = head + number_of_heads
 *                  * ((sector - 1) + sectors_per_track * track)
 */
#include <keffos/config.h>
#include <keffos/stdio.h>
#include <keffos/magic.h>
#include <keffos/errno.h>
#include <keffos/klog.h>
#include <keffos/blockdev.h>
#include <sys/defines.h>
#include <asm/io.h>

enum { DEVICE_HDA, DEVICE_HDB, DEVICE_HDC, DEVICE_HDD };

static int ide_ioctl(int device, int request, ...);
static int ide_write(int device, __uint64 block, void *buf, __uint64 count);
static int ide_read(int device, __uint64 block, void *buf, __uint64 count);
static int init_module(int argc, const char **argv);
static int exit_module(int force);

static int ide_devices[2][2] = {{0,0},{0,0}};

static block_device_t bd_ide = {
	BD_DRIVER_MAGIC,
	"ide",
	&ide_read,
	&ide_write,
	&ide_ioctl
};

/*
 *
 */
static int ide_ioctl(int device, int request, ...)
{
	switch (device) {
	case DEVICE_HDA:
		break;
	case DEVICE_HDB:
		break;
	case DEVICE_HDC:
		break;
	case DEVICE_HDD:
		break;
	default:
		syslog(LOG_INTERNAL_ERROR, "ide_ioctl(): unknown device");
		return -(errno = EFIXME);
		break;
	}
	return -(errno = ENOIOCTLCMD);
}

/*
 *
 */
static int ide_write(int device, __uint64 block, void *buf, __uint64 count)
{
	switch (device) {
	case DEVICE_HDA:
		break;
	case DEVICE_HDB:
		break;
	case DEVICE_HDC:
		break;
	case DEVICE_HDD:
		break;
	default:
		syslog(LOG_INTERNAL_ERROR, "ide_write(): unknown device");
		return -(errno = EFIXME);
		break;
	}
	return 0; // no bytes written
}

/*
 *
 */
static int ide_read(int device, __uint64 block, void *buf, __uint64 count)
{
	switch (device) {
	case DEVICE_HDA:
		break;
	case DEVICE_HDB:
		break;
	case DEVICE_HDC:
		break;
	case DEVICE_HDD:
		break;
	default:
		syslog(LOG_INTERNAL_ERROR, "ide_read(): unknown device");
		return -(errno = EFIXME);
		break;
	}
	return 0; // no bytes read
}

/*
 *
 */
static int init_module(int argc, const char **argv)
{
	int found = 0;
	int ch;
	
	ch = inb(0x1f7);
	if (ch != 0x7e) {
		syslog("Found IDE primary master\n");
		ide_devices[0][0] = 1;
		register_block_device("hda", DEVICE_HDA, &bd_ide);
		found++;
	}
	
	ch = inb(0x3f7);
	if (ch != 0x7e) {
		syslog("Found IDE primary slave\n");
		ide_devices[0][1] = 1;
		register_block_device("hdb", DEVICE_HDB, &bd_ide);
		found++;
	}
	
	return (found) ? 0 : -1;
}

/*
 *
 */
int exit_module(int force)
{
  
}
