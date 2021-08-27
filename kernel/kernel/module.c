/* $Id: module.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * Module loading and unloading
 *
 * Thomas Habets         -               Initial coding
 *
 */
#include <keffos/config.h>
#include <keffos/misc.h>

int module_init(void)
{
#if CONFIG_NET
	features |= FEATURE_NET;
#endif
}
