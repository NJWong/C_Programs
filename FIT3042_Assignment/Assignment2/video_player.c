#include <stdio.h>
#include <string.h>
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

// returns 0 on success, -1 on failure
int read_ppm_frame_header(int *dimensions)
{
    char next_c = '\0';
    char *line_buffer = NULL;
    int line_buffer_size = 0;

    /* Read and validate the 'magic number' line */
    line_buffer_size = 3;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    // TODO move this to a function
    if (line_buffer == NULL)
    {
        printf("Error: line_buffer is NULL.\n");
        return -1;
    }

    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    int i = 0;

    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    /* Fail if magic number is not 'P6' */   
    if (strcmp(line_buffer, "P6") != 0)
    {
        printf("Not a valid PPM file! Magic number line was: %s\n", line_buffer);
        return -1;
    }

    free(line_buffer);

    /* Allocate memory for the dimensions - reasonable to assume this is enough */
    line_buffer_size = 5;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    /* SET WIDTH */
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    /* Read in the width dimension to line_buffer */
    i = 0;
    while ((next_c = fgetc(stdin)) != ' ')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    dimensions[0] = atoi(line_buffer);

    /* SET HEIGHT */
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    /* Read in the height dimension to line_buffer */
    i = 0;
    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    dimensions[1] = atoi(line_buffer);

    free(line_buffer);



    /* If dimensions are NULL, then set them */
    /* Otherwise, check the values are consistent */
    /* Fail if values are not consistent */

    return 0;
}

// return 0 on success, -1 on failure
int play_video(char **argv)
{
    printf("Playing video at %s delay\n", argv[1]);

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    int screen_width = NULL;
    int screen_height = NULL;

    /* Initialize the dimensions array*/
    int *dimensions = (int *) malloc(3*sizeof(int));
    memset(dimensions, 0, 2);
    dimensions[2] = '\0';

    /* Read in the ppm frame header */
    if (read_ppm_frame_header(dimensions) != 0)
    {
        return -1;
    }

    /* Get screen dimensions from the current .ppm image */
    screen_width = dimensions[0];
    screen_height = dimensions[1];

    printf("%d\n", screen_width);
    printf("%d\n", screen_height);


    /* TODO check that screen_width and screen_height are not NULL */

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
            SDL_Delay(1000); // TODO remove this
        }

        /* Clean up */
        // close(window, screenSurface);
        SDL_FreeSurface(screenSurface);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}

