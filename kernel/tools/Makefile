# $Id: Makefile,v 1.2 2001/05/31 21:32:23 marvin Exp $
#
TARGET=postmake mkramfs sysmap
all: $(TARGET)

%.o: %.c
	gcc -Werror -w -pipe -g -c -o $@ $< -I../include

mkramfs: mkramfs.o
	gcc -g -o $@ $<

postmake: postmake.o
	gcc -g -o $@ $<

sysmap: sysmap.o
	gcc -g -o $@ $<

clean:
	rm -f *.o $(TARGET)
