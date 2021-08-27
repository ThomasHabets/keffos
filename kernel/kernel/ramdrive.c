/* $Id: ramdrive.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * initrd
 *
 */
#include <keffos/config.h>
#include <keffos/vfs.h>
#include <keffos/errno.h>
#include <keffos/stdio.h>
#include <keffos/magic.h>
#include <keffos/elf.h>
#include <keffos/ramdrive.h>
#include <keffos/string.h>
#include <keffos/module.h>
#include <keffos/ctype.h>
#include <keffos/symbols.h>

#define SYSTEM_MAP_NAME "System.map"
#define RAMDRIVE_LOGICAL_START ((0x300 << 22) + 0x70000)

int ramdrive_loadall(void)
{
	/* now the ramdrive is loaded at 0x70000 (logical 0x40000 to kernel) */
	//  Elf32_Ehdr *hdr = (Elf32_Ehdr*)0x40000;
	ramdrive_entry_t *entry;
	char buffer[128];
	int flen;
	char fname[128];
	
	flen = *((unsigned char*)RAMDRIVE_LOGICAL_START + 4);
	strncpy(fname, (char*)(RAMDRIVE_LOGICAL_START + 5), 128);
	entry = (ramdrive_entry_t*)(RAMDRIVE_LOGICAL_START + 5 + flen);
#if 1
	printk("fnamelen: %d\n", flen);
	printk("file: %s\n", fname);
#endif
	if ((flen != strlen(SYSTEM_MAP_NAME))
	    || (strncmp(SYSTEM_MAP_NAME, fname, strlen(SYSTEM_MAP_NAME)))) {
		panic("ramdrive_loadall(): first file in ramdrive != "
		      SYSTEM_MAP_NAME);
	}
	
	loadsymbols((char*)(RAMDRIVE_LOGICAL_START + 5 + flen
			    + sizeof(ramdrive_entry_t)), entry->size);
	printk("Loading initial modules...\n");
	return 0;
}
