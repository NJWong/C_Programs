CC 	   		 = gcc
CFLAGS 		 = -g -Wall -I.
MODE   		 = -std=c99
INCLUDE_SDL2 = -I/usr/include/SDL2
LINK_SDL2    = -lSDL2
LINK_MATH	 = -lm

all: clean_bins ppmplayer

ppmplayer: arg_parser.c video_player.c ppmplayer.c driver.c
	$(CC) $(INCLUDE_SDL2) $(CFLAGS) $(MODE) -o ppmplayer arg_parser.c video_player.c ppmplayer.c driver.c $(LINK_SDL2) $(LINK_MATH)

clean: clean_bins clean_ppm clean_rle

# Clean up compiled executable for ppmplayer
clean_bins:
	$(RM) ppmplayer

# Clean up generated .ppm files in root directory
clean_ppm:
	$(RM) ./*.ppm

# Clean up generated .rle files in root directory
clean_rle:
	$(RM) ./*.rle