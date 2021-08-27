/* $Id: elf.h,v 1.2 2001/05/31 21:32:21 marvin Exp $
 */
#ifndef __INCLUDE_ELF_H__
#define __INCLUDE_ELF_H__

#include <sys/defines.h>

#pragma pack(1)
typedef struct Elf32_Ehdr_s {
	unsigned char ident[16];
	__uint16 type;                 /* Object file type */
	__uint16 machine;              /* Architecture */
	__uint32 version;              /* Object file version */
	__uint32 entry;                /* Entry point virtual address */
	__uint32 phoff;                /* Program header table file offset */
	__uint32 shoff;                /* Section header table file offset */
	__uint32 flags;                /* Processor-specific flags */
	__uint16 ehsize;               /* ELF header size in bytes */
	__uint16 phentsize;            /* Program header table entry size */
	__uint16 phnum;                /* Program header table entry count */
	__uint16 shentsize;            /* Section header table entry size */
	__uint16 shnum;                /* Section header table entry count */
	__uint16 shstrndx;             /* Section header string table index */
} Elf32_Ehdr;
#pragma unpack

#endif
