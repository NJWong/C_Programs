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

int check_valid_ppm(char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    int i = 0;
    char next_c = '\0';

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

    return 0;
}

void set_width(int *dimensions, char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    /* Read in the width dimension to line_buffer */
    char next_c = '\0';
    int i = 0;
    while ((next_c = fgetc(stdin)) != ' ')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    dimensions[0] = atoi(line_buffer);
}

void set_height(int *dimensions, char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    /* Read in the height dimension to line_buffer */
    char next_c = '\0';
    int i = 0;
    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    dimensions[1] = atoi(line_buffer);
}

int check_valid_max_val(char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    int i = 0;
    char next_c = '\0';

    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    /* Fail if magic number is not 'P6' */   
    if (strcmp(line_buffer, "255") != 0)
    {
        printf("Not a valid max val: %s\n", line_buffer);
        return -1;
    }

    return 0;
}

// returns 0 on success, -1 on failure
int read_ppm_frame_header(int *dimensions)
{
    char *line_buffer = NULL;
    int line_buffer_size = 0;
    
    /* CHECK VALID PPM */
    line_buffer_size = 3;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: line_buffer is NULL.\n");
        return -1;
    }

    if (check_valid_ppm(line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

    free(line_buffer);

    /* Allocate memory for the dimensions - reasonable to assume this is enough */
    line_buffer_size = 5;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: line_buffer is NULL.\n");
        return -1;
    }

    /* SET WIDTH */
    set_width(dimensions, line_buffer, line_buffer_size);

    /* SET HEIGHT */
    set_height(dimensions, line_buffer, line_buffer_size);

    free(line_buffer);


    /* CHECK MAX VALUE - should be 255 */
    line_buffer_size = 4;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: line_buffer is NULL.\n");
        return -1;
    }

    if (check_valid_max_val(line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

    free(line_buffer);

    return 0;
}

int display_frame(SDL_Surface *screenSurface, SDL_Window *window, int screen_width, int screen_height)
{
    Uint8 red_channel = 0;
    Uint8 green_channel = 0;
    Uint8 blue_channel = 0;
    /* Display the next frame on the screen surface */
    for (int y = 0; y < screen_height; y++)
    {
        for (int x = 0; x < screen_width; x++)
        {
            int *p = screenSurface->pixels + y * screenSurface->pitch + x * screenSurface->format->BytesPerPixel;

            // red_channel = get_next_red();
            // green_channel = get_next_green();
            // blue_channel = get_next_blue();
            red_channel = fgetc(stdin);
            green_channel = fgetc(stdin);
            blue_channel = fgetc(stdin);

            *p=SDL_MapRGB(screenSurface->format, red_channel, green_channel, blue_channel);
        }
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(1000); // TODO remove this
    return 0;
}

// return 0 on success, -1 on failure
int play_video(int screen_width, int screen_height)
{
    /* Create an SDL window using the .ppm dimensions */
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("ppmplayer",
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

            display_frame(screenSurface, window, screen_width, screen_height);
        }

        /* Clean up */
        // close(window, screenSurface);
        SDL_FreeSurface(screenSurface);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}

int video_player_init(char **argv)
{
    printf("Playing video at %s delay\n", argv[1]);

    int *dimensions = NULL;
    int screen_width = 0;
    int screen_height = 0;

    /* Initialize the dimensions array*/
    dimensions = (int *) malloc(3*sizeof(int));
    if (dimensions == NULL)
    {
        printf("Error: dimensions array is NULL.\n");
        return -1;
    }
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

    free(dimensions);

    if (!screen_width || !screen_height)
    {
        printf("Error: screen height or width is 0.\n");
        return -1;
    }

    if(play_video(screen_width, screen_height) != 0)
    {
        return -1;
    }
    return 0;
}