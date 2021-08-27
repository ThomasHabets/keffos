/* $Id: mkkeffs.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * Create a keffs filesystem
 *
 */
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <keffs.h>

#define VERSION "0.1"
#define KEFFS_REV 0
#define MIN_BLOCKSIZE 1024
#define MAX_BLOCKSIZE 8192

int blocksize = 1024;
char check = 0;
char reserved_blocks = 0;
char reserved_percent = 5;
char *creator_os = "KeffOS";
char verbose = 0;
char *label = "";
char *last_mountpoint = "";
int revision = KEFFS_REV;
char undelete = 0;
char journaling = 0;
char dirty = 0;

char *programname;
char *device_name;

keffs_superblock_t super;


int log2(int arg)
{
	int	l = 0;
  
	arg >>= 1;
	while (arg) {
		l++;
		arg >>= 1;
	}
	return l;
}

int pow2(int arg)
{
	int c;
	int ret = 2;
	for (c = arg; c-1; c--) {
		ret *= 2;
	}
	return ret;
}

void usage2(void)
{
	fprintf(stderr, "%s [ -options ] device\n"
		"Make a KefFS filesystem\n\n"
		"  -h                         show this help\n" 
		"  -c                         check device for errors\n"
		"  -b <blocksize>             blocksize (default %d)\n"
		"  -o <name>                  creator os (default \"%s\")\n"
		"  -L <label>                 filesystem label (default: none)\n"
		"  -M <path>                  last mounted directory (default: none)\n"
		"  -v                         verbose\n"
		"  -j                         turn on journaling\n"
		"  -d                         mark filesystem as dirty\n"
		"  -u                         turn on undelete feature\n"
		"  -V                         output version information and exit\n"
		"  -m <percent>               reserved blocks, 0->100 (default: 5)\n"
		"  -n <block count>           reserved blocks, overrides -m\n"
		"\n"
		"  device                     the device or file to make the filesystem on\n",
		programname, blocksize, creator_os);
	exit(0);
}

void usage(void)
{
	fprintf(stderr, "%s [ -c ] [ -b block-size ] [ -o creator-os ]"
		"\n\t[ -L label ] [ -M last-mounted-directory ] [ -vjduV ]"
		"\n\t[ -m reserved-blocks-percentage | -n reserved-blocks ]"
		"\n\tdevice\n", programname);
	exit(0);
}

void parms(int argc, char **argv)
{
	int c;
	char *tmp;

	if ((argc > 1) && !strcmp(argv[1], "--advice")) {
		printf("Don't Panic!\n");
		exit(42);
	}

	while ((c = getopt(argc, argv, "b:cm:n:o:vL:M:r:jduVh")) != EOF) {
		switch (c) {
		case 'b': /* block size */
			blocksize = strtoul(optarg, &tmp, 0);
			if (blocksize < MIN_BLOCKSIZE || blocksize > 4096 || *tmp
			    || (pow2(log2(blocksize)) != blocksize)) {
				fprintf(stderr, "%s: bad block size - %s\n", programname, optarg);
				exit(1);
			}
			break;
		case 'm': /* reserve percent */
			reserved_blocks = 0;
			reserved_percent = strtoul(optarg, &tmp, 0);
			if (reserved_percent > 100 || *tmp) {
				fprintf(stderr, "%s: bad reserved block percent - %s", programname, optarg);
				exit(1);
			}
			break;
		case 'n': /* reserve blocks */
			reserved_percent = 0;
			reserved_blocks = strtoul(optarg, &tmp, 0);
			if (*tmp) {
				fprintf(stderr, "%s: bad reserved block count - %s", programname, optarg);
				exit(1);
			}
			break;
		case 'o': /* creator os */
			creator_os = optarg;
			break;
		case 'v': /* verbose */
			verbose = 1;
			break;
		case 'L': /* label */
			label = optarg;
			break;
		case 'M': /* last mount */
			last_mountpoint = optarg;
			break;
		case 'j':
			fprintf(stderr, "%s: this version of mkkeffs does not support journaling\n", programname);
			exit(1);
		case 'd':
			dirty = 1;
			break;
		case 'u':
			fprintf(stderr, "%s: this version of mkkeffs does not support undelete\n", programname);
			exit(1);
		case 'h':
			usage2();
		case 'V': /* version */
			fprintf(stderr, "mkkeffs %s for KefFS rev %d\n", VERSION, KEFFS_REV);
			exit(0);
		case 'c': /* check */
			check = 1;
			break;
		case ':':
		case '?':
			exit(1);
			break;
		default:
			fprintf(stderr, "'%c'\n", c);
			fprintf(stderr, "CAN'T HAPPEN!!!\n");
			exit(1);
		}
	}
	if (optind == argc) {
		usage();
	}
	device_name = argv[optind++];
}

void create_root_inode(keffs_inode_t *inode)
{
	inode->type = KEFFS_TYPE_DIR;
	inode->mode = 0755;
	inode->uid = 0;
	inode->gid = 0;
	inode->size = 2;  /* '.' and '..' */
	inode->levels = 0;
	inode->flags = 0;
	inode->links_count = 1; /* the name '/' */
	inode->atime = time(0);
	inode->btime = time(0);
	inode->ctime = time(0);
	inode->mtime = time(0);
	inode->dtime = time(0);
}

void create_super(int blocksize, int blocks)
{
	if (sizeof(keffs_superblock_t) != 512) {
		fprintf(stderr, "%s: Bleh, superblock != 512 bytes (its %d bytes)\n",programname, sizeof(keffs_superblock_t));
		exit(1);
	}
	bzero(super, sizeof(keffs_superblock_t));
	memcpy(super.idstr, KEFFS_IDSTR, KEFFS_IDLEN);
	super.revision = 0;
	super.id = (__uint64)rand() << 32 | rand();
	super.blocksize = blocksize;
	super.blocks = blocks;
	super.freeblocks = blocks - 1; /* root dir */
	super.filecount = 1;
	super.inodecount = 1;
	strncpy(super.label, label, KEFFS_LABEL_MAXSIZE); // FIXME: -1?
	strncpy(super.last_mountpoint, last_mountpoint, KEFFS_LASTMOUNT_MAXSIZE); // FIXME: -1?
	strncpy(super.creator_os, creator_os, KEFFS_CREATOR_OS_MAXSIZE);
	super.btime = time(0);
	super.atime = time(0);
	super.lastchecktime = time(0);
	super.maxchecktime = 3600 * 24 * 30 * 6; /* 6 months */
	super.mountcount = 0;
	super.maxmountcount = 20; /* count before check forced FIXME: make switch for this */
	super.reserved_blocks = (reserved_blocks) ? reserved_blocks : (blocks * reserved_percent) / 100;
	super.error = 0; /* == continue */
	super.flags = (KEFFS_FLAG_DIRTY & dirty);
	/*
	  | (KEFFS_FLAG_JOURNALING & journaling);
	  | (KEFFS_FLAG_UNDELETE & undelete);
	*/
	super.root = 0; /* wrong! FIXME: block # of root inode */
}

void write_super(FILE *f, __uint64 blocks)
{
	char *buf;
	fwrite(&super, sizeof(keffs_superblock_t), 1, f); /* second half of block 0 */
	if (!(buf = (char*)malloc(blocks / 8))) {
		perror("malloc()");
		exit(1);
	}
	(void)memset((void*)buf, 0, blocks / 8); /* mark all blocks as free */
	/* FIXME: set superblock space as in-use and root inode as inode and in-use */
	fwrite(buf, blocks / 8, 1, f); /* free blocks */
	fwrite(buf, blocks / 8, 1, f); /* inodes */
	free(buf);  
}

int main(int argc, char **argv)
{
	FILE *f;
	struct stat s;
	__uint64 size;
	__uint64 blocks;
	char buffer[512];
	keffs_inode_t inode;
	keffs_direntry_t direntry;
  
	programname = argv[0];
	parms(argc, argv);
  
	if (!(f = fopen(device_name, "r+"))) {
		fprintf(stderr, "%s: unable to open device %s\n", programname, device_name);
		exit(1);
	}

	fstat(fileno(f), &s);
	printf("device size: %d\n", s.st_size);
	size = s.st_size - (s.st_size % blocksize);
	blocks = size / blocksize - 1; /* -1 for superblock */

	if (size % (blocksize * 8)) {
		fprintf(stderr, "%s: FIXME: size must be multiple of blocksize * 8\n", programname);
		exit(1);
	}

	bzero(buffer, 512);
	fwrite(buffer, 512, 1, f);

	printf("Making superblocks (size: %d * %d bytes)\n", blocksize, sizeof(keffs_superblock_t) + (blocks / 8) * 2);
	create_super(blocksize, size / blocksize);
	printf("@ %d", ftell(f)); fflush(stdout);
	write_super(f, blocks);

	fseek(f, size / 2, SEEK_SET);
	printf(", %d", ftell(f)); fflush(stdout);
	write_super(f, blocks);

	fseek(f, size - 2*(blocks / 8) - sizeof(keffs_superblock_t), SEEK_SET);
	printf(", %d\n", ftell(f)); fflush(stdout);
	write_super(f, blocks);
  
	/*
	 * FIXME: mark blocks as in-use:
	 *  superblock blocks
	 *  root inode
	 */

	/* FIXME: round-off error in divide right? */
	fseek(f, blocksize + (blocks / 8) / blocksize, SEEK_SET);
	printf("Making root inode\n");
	create_root_inode(&inode);
	fwrite(&inode, sizeof(keffs_inode_t), 1, f);

	direntry.block = 1;
	direntry.name_len = 1;
	fwrite(&direntry, sizeof(keffs_direntry_t), 1, f);
	fwrite(".", sizeof(keffs_direntry_t), 1, f);
	direntry.name_len = 2;
	fwrite(&direntry, sizeof(keffs_direntry_t), 1, f);
	fwrite("..", sizeof(keffs_direntry_t), 1, f);
  
	return 0;
}
