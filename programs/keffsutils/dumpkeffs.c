/* $Id: dumpkeffs.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * Dump information on a keffs filesystem
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <keffs.h>

char *programname;

void usage(void)
{
	fprintf(stderr, "%s [ -bVh ] device\n", programname);
	exit(0);
}

void die(const char *msg, const char *parm)
{
	fprintf(stderr, "%s: %s: %s\n", programname, msg, parm);
	exit(1);
}

void dumpfs(FILE *f)
{
	keffs_superblock_t super;
	char buffer[80];
	int c;
  
	fseek(f, 512, SEEK_SET);
	fread(&super, sizeof(keffs_superblock_t), 1, f);
  
	snprintf(buffer, 79, "ID string:          '%%.%ds'\n", KEFFS_IDLEN);
  
	printf(buffer, super.idstr);
	printf("Revision:           %d\n", super.revision);
	printf("id:                 0x%.8x-0x%.8x\n", super.id, super.id >> 32); /* looks wrong */
	printf("blocksize:          %d\n", super.blocksize);
	printf("blocks:             %d\n", super.blocks);
	printf("freeblocks:         %d\n", super.freeblocks);
	printf("filecounts:         %d\n", super.filecount);
	printf("inodecount:         %d\n", super.inodecount);
	printf("label:              '%s'\n", super.label);
	printf("last mountpoint:    '%s'\n", super.last_mountpoint);
	printf("creator OS:         '%s'\n", super.creator_os);
	printf("flags:             ");
	c = 0;
	if (super.flags & KEFFS_FLAG_DIRTY) {
		printf(" dirty");
		c++;
	}
	if (super.flags & KEFFS_FLAG_JOURNALING) {
		printf(" journaling");
		c++;
	}
	if (super.flags & KEFFS_FLAG_UNDELETE) {
		printf(" undelete");
		c++;
	}
	if (!c) {
		printf(" none");
	}
	printf("\n");
	printf("root inode number:  %d\n", super.root);
  
}

int main(int argc, char **argv)
{
	int c;
	FILE *f;

	programname = argv[0];

	while ((c = getopt(argc, argv, "hb-:V")) != EOF) {
		switch (c) {
		case '-':
			if (!strcmp(optarg, "help")) {
			case 'h':
				usage();
			} else {
				fprintf(stderr, "%s: unrecognized option '--%s'", programname, optarg);
				exit(1);
			}
			break;
		default:
			printf("CAN'T HAPPEN!!!!\n");
			exit(1);
			break;
		}
	}
	if (argc == optind) {
		usage();
	}
	if (!(f = fopen(argv[optind], "r"))) {
		perror("fopen(device)");
		exit(1);
	}
	dumpfs(f);
	fclose(f);
}
