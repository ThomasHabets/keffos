/* $Id: fingerd.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * finger daemon
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	int c;
	char *p_ch;
	char name[129];

	while (-1 != (c = getopt(argc, argv, "vd"))) {
		switch (c) {
		case 'v':
			printf("fingerd version 1.0\n");
			return 1;
		case 'd':
			printf("Daemon mode not supported yet\n");
			return 1;
		default:
			printf("Unknown option on cmdline\n");
			return 1;
		}
	}

	fgets(name, 128, stdin);
	if ((p_ch = strchr(name, '\n'))) {
		*p_ch = 0;
	}
  
	/*
	 * TODO:
	 *  check if user wants to be fingered
	 *  cat /etc/fingerd/header and /etc/fingerd/footer
	 *  cat planfile
	 *  cat pgpkey
	 *  fakemode:
	 *     if (-1 == finger(name@client.who.asked)) {
	 *       if (-1 == finger(name@idsoftware.com)) {
	 *         do_something()
	 *
	 */
	if (strchr(name, '@')) {
		// FIXME: check config file for forwarding
		printf("Finger forwarding service denied\n");
	} else if (name[0]) {
		// FIXME: handle a list of users/full names
		printf("No such user '%s'\n", name);
	} else {
		printf("Finger online user list denied\n");
	}
  
	// FIXME: log to syslog
	return 0;
}
