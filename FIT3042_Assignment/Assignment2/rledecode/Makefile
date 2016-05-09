CC=gcc
CFLAGS=-Wall -g  -std=gnu99

all:	rledecode

clean:
	rm *.o

clobber:
	make clean
	rm rledecode

rledecode: rledecode.o decompress.o
	$(CC) $(CFLAGS) -o rledecode rledecode.o decompress.o

# This target generates docs.  These packages should be installed:
# doxygen
# texlive
# texlive-latex-extra
docs: rledecode.c decompress.h
	doxygen RLEDoxyfile
	cd docs/latex && make
