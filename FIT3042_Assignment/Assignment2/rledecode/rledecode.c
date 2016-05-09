/** @file rledecode.c
 * @brief Decoder for RLEplay format video files.
 *
 * This sample solution is intended as a basis for students to tackle
 * Assignment 2 for FIT3042.  It does not implement scaling or tweening
 * as Assignment 2 does not rely on this functionality.
 *
 * @author Robyn A. McNamara
 * @date April 2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "decompress.h"


/**
 * @brief Prints a usage message to stderr.
 */
void usage() {
	fprintf(stderr, "Usage: rledecode input-file.rle [output-name | -]\n");
}

int main(int argc, char *argv[]) {

	// Validate command line arguments
	if (argc != 3) {
		usage();
		exit(1);
	}

	// First arg should represent a readable file
	char *filename = argv[1];
	FILE* infile;

	if (!(infile = fopen(filename, "r"))) {
		fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
		exit(1);
	}

	// Second arg may tell us to output to stdout
	char *outname = NULL;
	bool conout = (strcmp(argv[2], "-") == 0);
	if (!conout) 
		outname = argv[2];

	// arguments are valid, let's process the file
	decompress_file(infile, outname);

	// cleanup
	fclose(infile);
	return 0;
}
