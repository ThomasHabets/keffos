/* $Id: symbols.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * initrd
 *
 */
#include <keffos/config.h>
#include <keffos/symbols.h>

/*
 * read entry from symbol table file
 */
static int readit(const char *in, unsigned int *pvalue, char *out, int maxlen)
{
#define isdigit(a) ((a >= '0') && (a <= '9'))
#define ishexdigit(a) (isdigit(a)||((tolower(a)>='a') && (tolower(a) <= 'f')))
	
	unsigned int weight = 268435456;
	char *ptr;
	
	if ((strlen(in) < 9) || (in[8] != ' ')) {
		return 1;
	}
	
	*pvalue = 0;
	for (; *in != ' '; in++, weight /= 16) {
		if (isdigit(*in)) {
			*pvalue += weight * (*in - '0');
		} else if (ishexdigit(*in)) {
			*pvalue += weight * (tolower(*in) - 'a' + 10);
		} else {
			return 1;
		}
	}  
	in++;
	strncpy(out, in, maxlen - 1);
	if ((ptr = strchr(out, '\n'))) {
		*ptr = 0;
	}
	return 0;
}

/*
 * load symbols from file @ data, file length flen
 * Format is system.map format. See documentation.
 */
void loadsymbols(const char *data, int len)
{
	int pos;
	char *ptr;
	unsigned int address;
	char symbol[128];
	int c = 0;
	symbols_t *sp;
	
	printk("Loading kernel symbols...\n");
	
	sp = &kernel_symbols;
	
	for (ptr = data; (ptr < &data[len]); ) {
		if (*ptr == '.') {
			/* FIXME: handle code/data symbols */
			ptr = strchr(ptr, '\n') + 1;
			continue;
		}
		if (readit(ptr, &address, symbol, 128)) {
			panic("Error reading symbol entry");
		}
		//    printk"Address: 0x%x, Symbol: '%s'\n", address, symbol);
		if (!(sp->next = (symbols_t*)kmalloc(sizeof(symbols_t)))) {
			panic("out of mem allocating symbol struct");
		}
		sp->size = address - (unsigned int)sp->ptr; /* right? */
		sp = sp->next;
		sp->next = 0;
		if (!(sp->name = strndup(symbol, 128))) {
			panic("out of mem allocating symbol name\n");
		}
		sp->module = NULL; /* KERNEL */
		sp->ptr = (void*) address;
		/* size set above */

		/* refnumber, doesn't matter if in kernel cause kernel can't
		   be unloaded */
		sp->ref = 0;
		
		if (!(ptr = strchr(ptr, '\n') + 1)) {
			break;
		}
	}
}

