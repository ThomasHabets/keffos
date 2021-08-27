/* $Id: postmake.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 * Kernel image post-processor
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

unsigned short checksum16(unsigned short *buf, int num)
{
	unsigned int sum;

	for (sum = 0; num > 0; num--) {
		sum += *buf++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (unsigned short)~sum;
}

int main(int argc, char **argv)
{
	FILE *f;
	char *buffer;
	unsigned short checksum;
	struct stat s;

	if (argc != 2) {
		fprintf(stderr, "usage\n");
		exit(1);
	}
	if (!(f = fopen(argv[1], "r+"))) {
		perror("fopen(argv[1])");
		exit(1);
	}

	fstat(fileno(f), &s);

	fseek(f, 8, 0);
	fwrite(&s.st_size, 4, 1, f);
	fseek(f,0,0);

	fstat(fileno(f), &s);
	buffer = (char*)malloc(s.st_size + 10);
	if (-1 == fread(buffer, s.st_size, 1, f)) {
		perror("read()");
		exit(1);
	}
  
	checksum = checksum16((unsigned short*)buffer, s.st_size / 2);
	fseek(f, 4, 0);
	fwrite(&checksum, 2, 1, f);
	printf("Checksum: 0x%x\n", checksum);
  
	fclose(f);
	return 0;
}
