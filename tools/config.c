/* $Id: config.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	char buffer[1024];
	char head[1024];
	char name[1024];
	char value[1024];

	FILE *fin;
	FILE *fout;
	FILE *fhelp;
	int line = 0;
	int i;

	if (argc != 4) {
		fprintf(stderr, "Crap<%d>, syntax error\n%s <infile> <outfile> <helpfile>\n", argc, argv[0]);
		exit(1);
	}
  
	if (!(fin = fopen(argv[1], "rt"))) {
		perror("fopen(infile)");
		exit(1);
	}
	if (!(fout = fopen(argv[2], "wt"))) {
		perror("fopen(outfile)");
		exit(1);
	}

	fprintf(fout, "/*\n * This file was automagically generated, so keep your ugly paws OFF!\n */\n"
		"#ifndef __INCLUDE_CONFIG_H__\n#define __INCLUDE_CONFIG_H__\n");

	while (!feof(fin)) {
		line++;
		buffer[0] = 0;
		fgets(buffer, 1024, fin);
		//    printf(buffer);
		if (buffer[0] == '!' || !buffer[0]) {
			continue;
		}
		if (3 != (i = sscanf(buffer, "%s %s %s\n", head, name, value))) {
			if (i == 1) {
				continue;
			}
			fprintf(stderr, "Line %d: wrong number of args (got %d)\nLine:\n\t%s\n", line, i, buffer);
			exit(1);
		}
		if (strcmp(head, "#define")) {
			fprintf(stderr, "Line %d: parse error, expected '#define', got '%s'\nLine:\n\t%s\n", line, head, buffer);
			exit(1);
		}
		printf("Setting %s to %s\n", name, value);
		fprintf(fout, "%s", buffer);
	}
  
	fprintf(fout, "\n#endif\n");

	fclose(fin);
	fclose(fout);
	
	return 0;
}
