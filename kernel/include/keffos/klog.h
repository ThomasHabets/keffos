/* $Id: klog.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __INCLUDE_KLOG_H__
#define __INCLUDE_KLOG_H__

/*
 * This is 'int' throughout the kernel. According to K&R 
 * we have at least 16 bits to play with then, should be enough.
 * FIXME: change to __int32 or __int16 ? Nah...
 */
#define LOG_INTERNAL_ERROR 0x01       // someone fucked up
#define LOG_STRUCT_ERROR   0x02       // struct has wrong value
#define LOG_WARNING        0x80       // just info with prio

#endif
