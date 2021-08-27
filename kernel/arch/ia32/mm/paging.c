/* $Id: paging.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * Keffos
 *
 * paging
 *
 */
int page_to_linear(unsigned int pagetable, unsigned int frame,
		   unsigned int offset)
{
	return ((pagetable << 22) | (frame << 12) | offset);
}

void linear_to_page(unsigned int *pagetable, unsigned int *frame,
		    unsigned int *offset, unsigned int linear)
{
	*pagetable = linear >> 22;
	linear -= *pagetable << 22;
	
	*frame = linear >> 12;
	linear -= *frame << 12;
	
	*offset = linear;
}
