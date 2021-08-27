/* $Id: config2.c,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#include <stdio.h>

int line = 1;

void parseerr(const char *msg)
{
	fprintf(stderr, "Line %d: Parse error, %s\n", line, msg);
	exit(1);
}

int getword(char *buf, int len, FILE *f)
{
	int ch;
	int c;
	int in_quote = 0;
  
	buf[0] = 0;
	while (!buf[0]) {
		for (c = 0; c < len - 1; c++) {
			switch (ch = fgetc(f)) {
			case '"':
				in_quote = !in_quote;
				c--;
				break;
			case '\n':
				line++;
			case ' ':
			case '\t':
				if (in_quote) {
					buf[c] = ch;
					break;
				}
				buf[c] = 0;
				return ch;
				break;
			case EOF:
				buf[c] = 0;
				return 0;
			default:
				buf[c] = ch;
				break;
			}
		}
	}
}
  
int main(int argc, char **argv)
{
	FILE *fin;
	FILE *fout;
	char buffer[1024];
  
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
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

	while (getword(buffer, 1024, fin)) {
		if (!buffer[0]) {
			continue;
		} else if (!strcmp(buffer, "section")) {
			if (!getword(buffer, 1024, fin)) {
				parseerr("expected sectionname, got EOF");
			}
			printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
			       "New section: %s\n"
			       "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n", buffer);
		} else if (!strcmp(buffer, "choice")) {
			// choice "text" "Choice1|Choice2|Choice3" NAME <default> "help text"
			if (!getword(buffer, 1024, fin)) {
				parseerr("expected title, got EOF");
			}
			printf("%s:\n", buffer);
			if (!getword(buffer, 1024, fin)) {
				parseerr("expected choices, got EOF");
			}
			printf("\t0) not\n\t1) done\n\t2) yet\n");

			if (!getword(buffer, 1024, fin)) {
				parseerr("expected name, got EOF");
			}
			printf("%s> ", buffer);
			printf("\n");

			if (!getword(buffer, 1024, fin)) {
				parseerr("expected helptext, got EOF");
			}
		} else if (!strcmp(buffer, "if")) {
			int j;
			if (!(j = getword(buffer, 1024, fin))) {
				parseerr("expected name, got EOF");
			}
			if (j != '\n') {
				if (!(j = getword(buffer, 1024, fin)) || strcmp(buffer, "=")) {
					parseerr("expected '= something'");
				}
				if (!(j = getword(buffer, 1024, fin))) {
					parseerr("expected 'something'");
				}
			}
		} else if (!strcmp(buffer, "elif")) {
			;
		} else {
			char *keff;
			keff = (char*)malloc(strlen(buffer) + 512);
			sprintf(keff, "Unknown command '%s'", buffer);
			parseerr(keff);
		}
	}

	return 0;
}
