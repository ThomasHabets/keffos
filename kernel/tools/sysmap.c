/* $Id: sysmap.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 *
 * KeffOS
 *
 */
#include <stdio.h>

void findword(char *buffer, const char *word)
{
	for (;;) {
		fgets(buffer, 1024, stdin);
		if (!strncmp(buffer, word, strlen(word))) {
			return buffer;
		}
		if (feof(stdin)) {
			fprintf(stderr, "Oh, pewp, '%s' not found\n", word);
			exit(1);
		}
	}
}
int main(int argc, char **argv)
{
	char buffer[1024];
	int i,j;
  
	if (argc != 1) {
		fprintf(stderr, "no parameters please\n", argv[0]);
		exit(1);
	}
  
	findword(buffer, ".text");
	sscanf(&buffer[5], "%x %x", &i, &j);
	printf(".text %x %x\n", i, j);
	for (;;) {
		fgets(buffer, 1024, stdin);
		if (!strncmp("                0x", buffer, strlen("                0x"))) {
			sscanf(buffer, "%x %s", &i, buffer);
			printf("%.8x %s\n", i, buffer);
		} else if (!strcmp(buffer, ".fini\n")) {
			break;
		}
	}

	findword(buffer, ".rodata");
	sscanf(&buffer[7], "%x %x", &i, &j);
	printf(".rodata %x %x\n", i, j);

	findword(buffer, ".data");
	sscanf(&buffer[5], "%x %x", &i, &j);
	printf(".data %x %x\n", i, j);
	for (;;) {
		fgets(buffer, 1024, stdin);
		if (!strncmp("                0x", buffer, strlen("                0x"))) {
			sscanf(buffer, "%x %s", &i, buffer);
			printf("%.8x %s\n", i, buffer);
		} else if (!strcmp(buffer, ".data1\n")) {
			break;
		}
	}


	findword(buffer, ".bss");
	sscanf(&buffer[4], "%x %x", &i, &j);
	printf(".bss %x %x\n", i, j);
	for (;;) {
		fgets(buffer, 1024, stdin);
		if (!strncmp("                0x", buffer, strlen("                0x"))) {
			sscanf(buffer, "%x %s", &i, buffer);
			printf("%.8x %s\n", i, buffer);
		} else if (!strcmp(buffer, ".stab\n")) {
			break;
		}
	}

	return 0;
}
