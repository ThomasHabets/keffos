/* $Id: syscall.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_SYSCALL_H__
#define __INCLUDE_SYSCALL_H__

#include <sys/syscall.h>
#include <sys/defines.h>
#include <keffos/limits.h>

#define SYS_test         0x0
#define SYS_shutdown     0x1
#define SYS_read         0x2
#define SYS_write        0x3
#define SYS_mount        0x4
#define SYS_umount       0x5
#define SYS_open         0x6
#define SYS_close        0x7
#define SYS_chmod        0x8
#define SYS_chown        0x9
#define SYS_sleep        0xa

typedef struct syscall_parameters_s {
	void *p0;
	void *p1;
	
	size_t size;
	
	__uint8 i8_0;
	__uint8 i8_1;
	
	__uint16 i16_0;
	__uint16 i16_1;
	
	__uint32 i32_0;
	__uint32 i32_1;
	
	__uint32 i64_0;
	__uint32 i64_1;
} syscall_parameters_t;
#endif
