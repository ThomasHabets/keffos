/* $Id: process.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_PROCESS_H__
#define __INCLUDE_PROCESS_H__

#include <keffos/limits.h>

typedef struct process_s {
	__uint32 magic;
	int argc;
	char **argv;
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t start_time;
	time_t processing_time;
	/* FIXME: special privs */
	
	/*
	 * memory reserved?
	 * or is that in mm?
	 */
	ptrsize_t stack;	// more like 0x26 prolly
} process_t;

#endif
