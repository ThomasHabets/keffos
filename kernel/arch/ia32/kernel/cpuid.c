/* $Id: cpuid.c,v 1.2 2001/05/31 21:32:21 marvin Exp $
 *
 * KeffOS
 *
 * C part of CPUID
 *
 */
#include <keffos/config.h>
#include <keffos/cpuid.h>

void get_processor_info(processor_info_t *ret)
{
	ret->vendor = "Transmeta...not";
	ret->model = "Alpha-balpha-ultra-super-duper-mega-sparc";
	
	ret->cpuid = 6;    // p2 = family 6, right?
	ret->speed = 1600; // fast, isn't it? :)
	ret->cache = 192;  // now *that's* an unimpressive cache
	
	ret->flags = 0xF1465;    // mmx, 3dnow, cpu, f00f, etc..
	// 0xF1465 = l33t speak for flags :)
}
