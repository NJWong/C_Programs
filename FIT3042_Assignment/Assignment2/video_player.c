#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "video_player.h"

/* GLOBALS */
SDL_Window *WINDOW = NULL;
SDL_Surface *SCREEN_SURFACE = NULL;
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

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

    /* Fail if magic number is not 'P6' or 'P3' */   
    if (strcmp(line_buffer, "P6") != 0)
    {
        printf("Not a valid PPM file! Magic number line was: %s\n. Expected values: [P6]\n", line_buffer);
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
        printf("Not a valid max val: %s\n. Expected values: [255]\n", line_buffer);
        return -1;
    }

    return 0;
}

int check_width(char *line_buffer, int line_buffer_size)
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

    if (atoi(line_buffer) == SCREEN_WIDTH)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int check_height(char *line_buffer, int line_buffer_size)
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

    if (atoi(line_buffer) == SCREEN_HEIGHT)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

// returns 0 on success, -1 on failure
int read_ppm_header()
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
    if (check_width(line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

    /* Check height */
    if (check_height(line_buffer, line_buffer_size) != 0)
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

int display_frame()
{
    Uint8 red_channel = 0;
    Uint8 green_channel = 0;
    Uint8 blue_channel = 0;

    /* Display the next frame on the screen surface */
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            int *p = SCREEN_SURFACE->pixels + y * SCREEN_SURFACE->pitch + x * SCREEN_SURFACE->format->BytesPerPixel;

            red_channel = fgetc(stdin);
            green_channel = fgetc(stdin);
            blue_channel = fgetc(stdin);

            *p=SDL_MapRGB(SCREEN_SURFACE->format, red_channel, green_channel, blue_channel);
        }
    }

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

void peek_screen_dimensions()
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
    SCREEN_WIDTH = atoi(peek_buffer_2);
    SCREEN_HEIGHT = atoi(peek_buffer_3);
    printf("width: %d, height %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

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

int peek_next_char()
{
    /* Peek next character */
    int peek = 0;

    /* 80 == 'P' which is the start of a new frame */
    if ((peek = fgetc(stdin)) != 80)
    {
        printf("--- No more frames! ---\n");
        return -1;
    }

    ungetc(peek, stdin);
    return 0;
}

void frame_update_loop(void *arg)
{
    read_ppm_header();
    display_frame();
    SDL_UpdateWindowSurface(WINDOW);
    remove_frame_separator();
}

Uint32 timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent user;
    user.type = SDL_USEREVENT;
    user.code = 0;

    user.data1 = &frame_update_loop;
    user.data2 = NULL;

    event.user = user;
    SDL_PushEvent(&event);

    return interval;
}

void close()
{

}

int video_player_init(char **argv)
{
    printf("Playing video %sms delay.\n", argv[1]);

    int delay_ms = atoi(argv[1]);

    /* Peek the dimensions from the first frame header */
    peek_screen_dimensions();

    /* Check the dimensions are positive integers */
    if (SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0)
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
        WINDOW = SDL_CreateWindow("ppmplayer",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (WINDOW == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            /* Create the screen. Default screen colour is purple. */
            SCREEN_SURFACE = SDL_GetWindowSurface(WINDOW);
            SDL_FillRect(SCREEN_SURFACE, NULL, SDL_MapRGB(SCREEN_SURFACE->format, 0xff, 0x00, 0xff));

            SDL_Event event;
            void (*fptr)(void *);

            SDL_TimerID frame_update_timer = SDL_AddTimer(delay_ms, timer_callback, NULL);

            while (peek_next_char() == 0 && feof(stdin) == 0 && SDL_WaitEvent(&event)) {
                switch(event.type) {
                    case SDL_QUIT:
                        printf("--- Closing video ---\n");
                        SDL_RemoveTimer(frame_update_timer);
                        SDL_Quit();
                        return -1;
                    case SDL_USEREVENT:
                        fptr = event.user.data1;
                        void *arg = event.user.data2;
                        fptr(arg);
                    default:
                        break;
                }
            }

            SDL_FreeSurface(SCREEN_SURFACE);
        }

        /* Clean up */
        SDL_DestroyWindow(WINDOW);
    }

    SDL_Quit();
    return 0;
}

