/* $Id: limits.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 *  Name         10^x
 *  ------------------
 *  exa	         18
 *  peta         15
 *  tera         12
 *  giga          9
 *  mega          6
 *  kilo          3
 *  
 *  Name        From                   To	             Scifrom     Scito
 *  --------------------------------------------------------------------------
 *  __int32              -2147483648            2147483647    -2.1e9     2.1e9
 *  __uint32                       0            4294967297         0     4.3e9
 *  
 *  __int64     -9223372036854775808   9223372036854775807   -9.2e18    9.2e18
 *  __uint64                       0  18446744073709551615         0    1.8e19
 *  
 *  ==========================================================================
 *  
 */
#ifndef __INCLUDE_LIMITS_H__
#define __INCLUDE_LIMITS_H__

#include <sys/defines.h>

#define pid_t __uint32

#define uid_t __uint32
#define gid_t __uint32

#define partitionsize_t __uint64
#define filesize_t      __uint64

#define semaphore_t     __uint32
#define filedesc_t      __uint32

#define size_t __uint64

// FIXME: is this fundamentally wrong?
#define time_t __uint64

#endif
