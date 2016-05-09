CC 	   = gcc
CFLAGS = -g -Wall -I.
MODE   = -std=c99

all: clean_bins ppmplayer

ppmplayer: arg_parser.c ppmplayer.c driver.c
	$(CC) $(CFLAGS) $(MODE) -o ppmplayer arg_parser.c ppmplayer.c driver.c

clean: clean_bins clean_output

# Clean up output ppm files
clean_output:
	$(RM) ./*.ppm

# Clean up compiled output files for program files
clean_bins:
	$(RM) ppmplayer