/* $Id: mutex.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * mutex code
 *
 * Thomas Habets         -                Initial coding
 *
 */
#include <keffos/config.h>
#include <keffos/mutex.h>
#include <asm/misc.h>

/*
 * suspend process until either
 * timout or mutex frees.
 * Return 1 if mutex aquired, else 0.
 */
int wait(int timeout, mutex_t m)
{
#if 0 /* pseudo-code */
	for(;;) {
		/*
		 * FIXME: if run a second time, 
		 * the timeout is too large
		 */
		if (!wait_no_aquire(timeout, m)) {
			return 0; /* timeout */
		}
		if (aquire_mutex(m)) {
			break;
		}
	}
	
#endif /* pseudo-code */
	return 1;
}

/*
 * get mutex
 */
int aquire_mutex(mutex_t m)
{
	int f;
	save_flags(f);
	cli();
	
#if 0 /* pseudo-code */
	/*
	 * last line of defence for race contitions.
	 * This code is run in cli mode so it should
	 * be immune to that sort of thing, though there is some instruction
	 * that works around it... hmm...
	 */
	if (mutex_active(m)) {
		return 0;
	}
	lock_mutex(m);
#endif /* pseudo-code */
	
	restore_flags(f);
}

/*
 * mark process in process list and suspend it
 */
int wait_no_aquire(int timeout, mutex_t m)
{
#if 0 /* pseudo-code */
	mark_process_as_waiting_for_mutex(self, m);
	if (timeout) {
		mark_process_as_waiting_for_timeout(self, timeout);
	}
	suspend(self);
#endif /* pseudo-code */
	
	return 1;
}

/*
 * request several mutexes at once.
 * protection against deadlocks
 */
int wait2(int timeout, mutex_t m, ...)
{
#if 0 /* pseudo-code */
	for (;;) {
		wait_no_aquire(m);           /* this is where time goes */
		if (check_other_mutexes()) {
			aquire_mutexes(all);
		}
	}
#endif /* pseudo-code */
	return 1;
}

/*
 * free mutex... er, technically correct?
 */
void signal(mutex_t m)
{
}
