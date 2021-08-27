/* $Id: mkramfs.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * Thomas Habets       -                Initial coding
 *
 */
#include <stdio.h>
#include <sys/stat.h>
#include <keffos/config.h>
#include <keffos/ramdrive.h>

#define BSIZE 1024

int main(int argc, char **argv)
{
	FILE *ofile;
	FILE *ifile;
	int c;
	char buffer[2048];
	int i;
	char *p;
	ramdrive_entry_t rde;
	char ch;
	struct stat s;

	if (argc < 3) {
		fprintf(stderr, "%s <outfile> <infile> [ infile ... ]\n", argv[0]);
		exit(1);
	}
  
	if (!(ofile = fopen(argv[1], "w"))) {
		perror("fopen()");
		exit(1);
	}

	//i = (int)htonl(CONFIG_RAMDRIVE_SIGNATURE);
	fwrite("KoRD", 4, 1, ofile);

	for (c = 2; c < argc; c++) {
		printf("Adding file %s\n", argv[c]);
		if (!(ifile = fopen(argv[c], "r"))) {
			perror("fopen()");
			exit(1);
		}
		if (!(p = (char*)strrchr(argv[c], '/'))) {
			p = argv[c];
		}
		fstat(fileno(ifile), &s);
		rde.permissions = 0700;
		rde.uid = 0;
		rde.gid = 0;
		rde.size = s.st_size;
		rde.flags = 0;
		ch = strlen(p);
		fwrite(&ch, 1, 1, ofile);
		fwrite(p, strlen(p), 1, ofile);
		fwrite(&rde, sizeof(ramdrive_entry_t), 1, ofile);
		for(;!feof(ifile);) {
			if (!(i = fread(buffer, 1, BSIZE, ifile))) {
				if (!feof(ifile)) {
					perror("fread()");
					exit(1);
				}
			}
			if (!(fwrite(buffer, i, 1, ofile))) {
				perror("fwrite()");
				exit(1);
			}
		}
		fclose(ifile);
	}
	ch = 0;
	fwrite(&ch, 1, 1, ofile);
	fclose(ofile);
  
}
