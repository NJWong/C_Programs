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

int check_width(int screen_width, char *line_buffer, int line_buffer_size)
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

    if (atoi(line_buffer) == screen_width)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int check_height(int screen_height, char *line_buffer, int line_buffer_size)
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

    if (atoi(line_buffer) == screen_height)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

// returns 0 on success, -1 on failure
int read_ppm_header(int screen_width, int screen_height)
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

    /* Check width */
    if (check_width(screen_width, line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

    /* Check height */
    if (check_height(screen_height, line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

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

int display_frame(SDL_Surface *screenSurface, SDL_Window *window, int screen_width, int screen_height, int delay_ms)
{
    Uint8 red_channel = 0;
    Uint8 green_channel = 0;
    Uint8 blue_channel = 0;
    // int counter = 0;

    /* Display the next frame on the screen surface */
    for (int y = 0; y < screen_height; y++)
    {
        for (int x = 0; x < screen_width; x++)
        {
            int *p = screenSurface->pixels + y * screenSurface->pitch + x * screenSurface->format->BytesPerPixel;

            red_channel = fgetc(stdin);
            green_channel = fgetc(stdin);
            blue_channel = fgetc(stdin);

            *p=SDL_MapRGB(screenSurface->format, red_channel, green_channel, blue_channel);
            // counter++;
        }
    }

    // printf("counter: %d\n", counter);

    SDL_UpdateWindowSurface(window);
    return 0;
}

void remove_frame_separator()
{
    int num_bytes = sizeof(int);
    for (int i = 0; i < num_bytes; i++)
    {
        fgetc(stdin);
    }
}

void peek_screen_dimensions(int *screen_width, int *screen_height)
{
    int peek_char = '\0';
    char *peek_buffer_1 = (char *) malloc(3*sizeof(char));
    char *peek_buffer_2 = (char *) malloc(5*sizeof(char));
    char *peek_buffer_3 = (char *) malloc(5*sizeof(char));

    memset(peek_buffer_1, '\0', 3);
    memset(peek_buffer_2, '\0', 5);
    memset(peek_buffer_3, '\0', 5);

    /* Magic Number */
    while ((peek_char = fgetc(stdin)) != '\n')
    {
        peek_buffer_1[strlen(peek_buffer_1)] = peek_char;
    }

    /* Width */
    while ((peek_char = fgetc(stdin)) != ' ')
    {
        peek_buffer_2[strlen(peek_buffer_2)] = peek_char;
    }

    /* Height */
    while ((peek_char = fgetc(stdin)) != '\n')
    {
        peek_buffer_3[strlen(peek_buffer_3)] = peek_char;
    }

    /* Get screen dimensions from the current .ppm image */
    *screen_width = atoi(peek_buffer_2);
    *screen_height = atoi(peek_buffer_3);
    printf("width: %d, height %d\n", *screen_width, *screen_height);

    /* Unpeek the first frame header */
    ungetc('\n', stdin);
    for (int i = strlen(peek_buffer_3); i > 0; i--)
    {
        ungetc(peek_buffer_3[i-1], stdin);
    }

    ungetc(' ', stdin);
    for (int i = strlen(peek_buffer_2); i > 0; i--)
    {
        ungetc(peek_buffer_2[i-1], stdin);
    }

    ungetc('\n', stdin);
    for (int i = strlen(peek_buffer_1); i > 0; i--)
    {
        ungetc(peek_buffer_1[i-1], stdin);
    }

    free(peek_buffer_1);
    free(peek_buffer_2);
    free(peek_buffer_3);
}

int video_player_init(char **argv)
{
    printf("Playing video %sms delay.\n", argv[1]);

    /* Create SDL Window */
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    /* Handle first ppm header */
    int screen_width = 0;
    int screen_height = 0;
    int delay_ms = atoi(argv[1]);

    /* Peek the dimensions from the first frame header */
    peek_screen_dimensions(&screen_width, &screen_height);

    /* Check the dimensions are positive integers */
    if (screen_width <= 0 || screen_height <= 0)
    {
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        /* Create the video player window using the dimensions */
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
            /* Create the screen. Default colour is purple */
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0x00, 0xff));




            for (int i = 0; i < 58; i++)
            {
                read_ppm_header(screen_width, screen_height);
                display_frame(screenSurface, window, screen_width, screen_height, delay_ms);
                remove_frame_separator();

                SDL_Delay(delay_ms); // TODO remove this

                /* Peek next character */
                int peek = fgetc(stdin);

                /* 80 = 'P' which is the start of a new frame */
                if (peek == 80)
                {
                    ungetc(peek, stdin);
                }
                else
                {
                    printf("We are at the end!\n");
                }
            }
        }

        /* Clean up */
        SDL_FreeSurface(screenSurface);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}

