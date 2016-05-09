/** decompress.c: functions that decompress RLEplay files.
 *   -- Robyn A. McNamara, April 2016
 */

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "decompress.h"

void decode_channel(RLEFile *file, uint8_t colour, uint8_t *buffer) {
	int pixcount =0;
	signed char checkbyte;
	uint8_t value;

	
	while(pixcount < file->pixels) {
		checkbyte = fgetc(file->fp);

		if(checkbyte >= 0) {
			for(int i = 0; i <= checkbyte; i++) // do n+1 times
			{
				value = fgetc(file->fp);
				set_channel_value(buffer, colour, pixcount++, value);
			}
		} else {
			value = fgetc(file->fp);
			for(int i = 0 ; i < (2 - checkbyte); i++)
				set_channel_value(buffer, colour, pixcount++, value);
		}
	}
	return;
}

void set_channel_value(uint8_t *buffer, uint8_t colour, int pixcount, uint8_t value) {
	// valid colour?
	if (colour > 2) {
		fprintf(stderr, "Invalid colour channel %u in set_channel_value -- expecting one of %u, %u, %u\n", colour, RED, GREEN, BLUE);
		exit(1);
	}

	buffer[(3 * pixcount) + colour] = value;
}


uint8_t* read_frame(RLEFile *file) {
	// allocate output buffer
	uint8_t *buffer = (uint8_t *)malloc(file->pixels * 3 * sizeof(uint8_t));
	if (!buffer) {
		fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
		exit(1);
	}

	// decode channels
	decode_channel(file, RED, buffer);
	decode_channel(file, GREEN, buffer);
	decode_channel(file, BLUE, buffer);

	return buffer;
}

void write_frame(RLEFile file, uint8_t *buffer, char *outname, int framenum) {
	FILE *outfile;
	// writing to stdout?
	if (!outname)
		outfile = stdout;
	else {
		char *outfilename = (char *)malloc((strlen(outname) + 12) * sizeof(char));
		sprintf(outfilename, "%s-%04d.ppm", outname, framenum);
		outfile = fopen(outfilename, "w");
		free(outfilename);
	}

	// do we have an output stream?
	if (!outfile) {
		fprintf(stderr, "Error opening output file\n");
		exit(1);
	}

	// write PPM header
	fprintf(outfile, "P6\n%d %d\n255\n", file.cols, file.rows); 

	// write buffer to output stream
	fwrite(buffer, 3*sizeof(uint8_t), file.pixels, outfile);
}

void decompress_file(FILE* infile, char *outname) {
	RLEFile *file = malloc(sizeof(RLEFile));
	if (!file) {
		fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
		exit(1);
	}

	// read and validate file structure
	file->fp = infile;
	char linebuf[100];
	fscanf(file->fp, "%100s", linebuf);

	// check for correct version string 
	if (strcmp(linebuf, "RLEPLAYv1") != 0) {
		fprintf(stderr, "input file is not RLEplay version 1\n");
		exit(1);
	}

	// read width and height
	fscanf(file->fp,"%d %d\n", &(file->cols), &(file->rows));
	file->pixels = file->cols * file->rows;

	// ready to parse the file
	int framenum = 1;
	bool firstframe = true;
	char marker = '\0';
	while((marker = fgetc(file->fp)) != 'E') {
		// valid frame start character?
		if (marker != 'K') {
			fprintf(stderr, "Invalid frame start character\n");
			exit(1);
		}

		// need to output separator character?
		if (!outname && !firstframe) {
			int sep = -1;
			fwrite(&sep, sizeof(int), 1, stdout);
		}

		// build frame and output it
		uint8_t *frame = read_frame(file);
		write_frame(*file, frame, outname, framenum);
		free(frame);
		framenum++;
		firstframe = false;
	}

	// free allocated memory
	free(file);
}
