#define mem48cpy(dselector, doffset, sselector, soffset, size) \
{ \
  __asm__ ("pushw %%es\n\t" \
	   "pushw %%ds\n\t" \
	   "movw %%ax, %%ds\n\t" \
	   "movw %%bx, %%es\n\t" \
	   "cld\n\t" \
	   "rep\n\t" \
	   "movsl\n\t" \
	   "popw %%ds\n\t" \
	   "popw %%es\n\t" \
	   : \
	   : "a" (dselector),"S" (soffset),"b" (dselector), "D" (doffset), "c" (size/4)); \
}

/*
 * WARNING: Hard coded for 2 B/char
 */
#define mem48setw(selector, offset, data, count) \
{ \
  __asm__ ("pushw %%es\n\t" \
	   "movw %%bx, %%es\n\t" \
	   "cld\n\t" \
	   "rep\n\t" \
	   "stosw\n\t" \
	   "popw %%es\n\t" \
	   : \
	   : "a" (data), "b" (selector), "D" (offset), "c" (count)); \
}
