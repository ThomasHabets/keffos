/* $Id: cpuid.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __INCLUDE_CPUID_H__
#define __INCLUDE_CPUID_H__

typedef struct processor_info_s {
	char *vendor;     /* "intel", "digital", "sun" */
	char *model;      /* "pentiumII", "K6" */
	char *submodel;   /* "Deschutes" */
	int cpuid;        /* generation */
	int speed;        /* Mhz */
	int cache;        /* in bytes */
#define CPU_FPU    0x01
#define CPU_MMX    0x02 
#define CPU_F00F   0x04
#define CPU_3DNOW  0x08
#define CPU_MTRR   0x10
	int flags;
} processor_info_t;

#endif
