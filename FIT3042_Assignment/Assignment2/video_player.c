#include <stdio.h>
#include <SDL.h>
#include "video_player.h"

int play(char **argv)
{
	printf("Playing video at %s delay\n", argv[1]);

	/* Open a file stream for the current .ppm image */

	/* Get screen dimensions from the current .ppm image */
	int screen_width = 800;
	int screen_height = 600;

	/* Create an SDL window using the .ppm dimensions */
	SDL_Window *window = NULL;

	SDL_Surface *screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("Hello World",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screen_width, screen_height,
			SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL,
				SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

			SDL_UpdateWindowSurface(window);

			SDL_Delay(2000);
		}

		/* Destroy the window */
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	return 0;
}

