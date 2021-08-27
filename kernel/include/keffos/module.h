/* $Id: module.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_MODULE_H__
#define __INCLUDE_MODULE_H__

#include <keffos/limits.h>

typedef struct modules_s {
	struct modules_s *next;
	char *name;
	void *loadpoint;
	ptrsize_t size;
	/*
	 * FIXME: insert list of all hooks module has made.
	 * used for unloading
	 */
} modules_t;

#endif
