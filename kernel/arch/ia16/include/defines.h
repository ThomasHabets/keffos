/* $Id: defines.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * Platform/compiler specific sizes.
 *
 */
#ifndef __INCLUDE_SYS_DEFINES_H__
#define __INCLUDE_SYS_DEFINES_H__

/* this is not really sys dependant but this is a good file to put it in */
#ifndef NULL
#define NULL ((void*)0)
#endif

#define __int8 char
#define __int16 int
#define __int32 long

#define __uint8  unsigned __int8
#define __uint16 unsigned __int16
#define __uint32 unsigned __int32

#define ptrsize_t __uint16

#endif
