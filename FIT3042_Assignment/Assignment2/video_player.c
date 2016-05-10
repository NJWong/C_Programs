#include <stdio.h>
#include <SDL.h>
#include "video_player.h"

int init(SDL_Window *window, SDL_Surface *screenSurface)
{
    return 0;
}

int close(SDL_Window *window, SDL_Surface *screenSurface)
{
    return 0;
}

int play_video(char **argv)
{
    printf("Playing video at %s delay\n", argv[1]);

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    /* Open a file stream for the current .ppm image */

    /* Get screen dimensions from the current .ppm image */
    int screen_width = 800;
    int screen_height = 600;

    /* Create an SDL window using the .ppm dimensions */
    

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
            /* Set the window surface to a purple/pink colour - easy to see for debugging */
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0x00, 0xff));

            /*  */
            for (int y = 0; y < screen_height; y++)
            {
                for (int x = 0; x < screen_width; x++)
                {
                    int *p = screenSurface->pixels + y * screenSurface->pitch + x * screenSurface->format->BytesPerPixel;

                    Uint8 red_channel = 0x00;
                    Uint8 green_channel = 0x00;
                    Uint8 blue_channel = 0x00;

                    *p=SDL_MapRGB(screenSurface->format, red_channel, blue_channel, green_channel);
                }
            }

            /* Blit the ppmSurface to the screenSurface */

            SDL_UpdateWindowSurface(window);
            SDL_Delay(2000); // TODO remove this
        }

        /* Clean up */
        // close(window, screenSurface);
        SDL_FreeSurface(screenSurface);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}

