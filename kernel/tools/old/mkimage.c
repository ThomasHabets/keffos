/*
 * KeffOS
 *
 *  
 *
 * Thomas Habets         -           Initial coding
 *
 */
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  FILE *fimage;
  FILE *fboot;
  FILE *fk;
  FILE *fkloader;
  struct stat s;
  char buffer[0x400];
  char *tmpbuf;
  int size = 0;

  if (argc != 5) {
    fprintf(stderr, "Usage: %s <image> <bootsect> <kloader> <kernel>\n", argv[0]);
    exit(1);
  }
  
  if (!(fimage = fopen(argv[1], "wb"))) {
    perror("fopen(image)");
    exit(1);
  }

  if (!(fboot = fopen(argv[2], "rb"))) {
    perror("fopen(bootsect)");
    exit(1);
  }
  
  if (!(fkloader = fopen(argv[3], "rb"))) {
    perror("fopen(kloader)");
    exit(1);
  }

  if (!(fk = fopen(argv[4], "rb"))) {
    perror("fopen(kernel)");
    exit(1);
  }

  fstat(fileno(fboot), &s);
  if (s.st_size != 0x200) {
    fprintf(stderr, "Bootsect != 0x200\n");
    exit(1);
  }

  if (-1 == fread(buffer, s.st_size, 1, fboot)) {
    perror("fread()");
    exit(1);
  }

  printf("Including bootector... %d bytes\n", 0x200);
  if (-1 == fwrite(buffer, 0x200, 1, fimage)) {
    perror("fwrite()");
    exit(1);
  }
  size += 0x200;

  ////// kloader
  fstat(fileno(fkloader), &s);
  if (s.st_size != 0x400) {
    fprintf(stderr, "kloader size != 0x400\n");
  }
  if (-1 == fread(buffer, s.st_size, 1, fkloader)) {
    perror("fread()");
    exit(1);
  }
  printf("Including kernel loader... %d bytes\n", s.st_size);
  if (-1 == fwrite(buffer, s.st_size, 1, fimage)) {
    perror("fwrite()");
    exit(1);
  }
  size += s.st_size;
  
  /////// kernel
  fstat(fileno(fk), &s);
  tmpbuf = (char*)malloc(s.st_size + 1);
  if (-1 == fread(tmpbuf, s.st_size, 1, fk)) {
    perror("fread()");
    exit(1);
  }
  printf("Including kernel... %d bytes\n", s.st_size);
  if (-1 == fwrite(tmpbuf, s.st_size, 1, fimage)) {
    perror("fwrite()");
    exit(1);
  }
  free(tmpbuf);
  size += s.st_size;
  

  fclose(fimage);
  fclose(fboot);
  fclose(fk);

  printf("Total image size: %d bytes\n", size);
  return 0;
}
