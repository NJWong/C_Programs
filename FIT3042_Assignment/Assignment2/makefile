CC 	   		 = gcc
CFLAGS 		 = -g -Wall -I.
MODE   		 = -std=c99
INCLUDE_SDL2 = -I/usr/include/SDL2
LINK_SDL2     = -lSDL2

all: clean_bins ppmplayer

ppmplayer: arg_parser.c video_player.c ppmplayer.c driver.c
	$(CC) $(INCLUDE_SDL2) $(CFLAGS) $(MODE) -o ppmplayer arg_parser.c video_player.c ppmplayer.c driver.c $(LINK_SDL2)

clean: clean_bins

# Clean up compiled executable for ppmplayer
clean_bins:
	$(RM) ppmplayer