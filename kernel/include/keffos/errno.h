/* $Id: errno.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __INCLUDE_ERRNO_H__
#define __INCLUDE_ERRNO_H__

#define	ENOSYS		0x01       /* Function not implemented */
#define EBUSY           0x02       /* Device or ressource busy */
#define EMAGIC          0x03       /* bad magic number. */
#define ENOMEM          0x04       /* out of mem */
#define EBADF           0x05       /* Bad file number */
#define EPERM           0x06
#define ENOENT          0x07
#define ENOIOCTLCMD     0x203      /* No ioctl command */

#define EFIXME          0xFFFF     /* Find correct err return of this err */

extern int errno;

const char *strerror(int err);

#endif
