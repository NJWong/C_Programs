#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // move this to the image_manipulator.c
#include <SDL.h>
#include "video_player.h"

/************* GLOBALS *************/

SDL_Window *WINDOW = NULL;
SDL_Surface *SCREEN_SURFACE = NULL;
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

/* Values for image manipulation - 50 is default */
int M_BRIGHTNESS = 50;
int M_CONTRAST = 50;
int M_SATURATION = 50;

/***********************************/

float find_max(float r, float g, float b)
{
    float max = r;

    if ((g > r) && (g > b))
    {
        max = g;
    }
    else if ((b > r) && (b > g))
    {
        max = b;
    }

    return max;
}

float find_min(float r, float g, float b)
{
    float min = r;

    if ((g < r) && (g < b))
    {
        min = g;
    }
    else if ((b < r) && (b < g))
    {
        min = b;
    }

    return min;
}

/*
    Algorithm adapted from:
    www.rapidtables.com/convert/rgb-to-hsv.htm
    www.cs.rit.edu/~ncs/color/t_convert.html
*/
void RGB_to_HSV(Uint8 r, Uint8 g, Uint8 b, float *h, float *s, float *v)
{
    /* Convert RGB values to floats */
    float r_float = (float)r;
    float g_float = (float)g;
    float b_float = (float)b;

    /* Divide by 255 to get values between 0 and 1 */
    r_float /= 255.0;
    g_float /= 255.0;
    b_float /= 255.0;

    float max = find_max(r_float, g_float, b_float);
    float min = find_min(r_float, g_float, b_float);

    float delta = max - min;

    /* CALCULATE THE HUE */
    if (delta == 0.0)
    {
        *h = 0;
    }
    else if (r_float == max)
    {
        *h = (g_float - b_float) / delta;
    }
    else if (g_float == max)
    {
        *h = 2 + ((b_float - r_float) / delta);
    }
    else
    {
        *h = 4 + ((r_float - g_float) / delta);
    }

    /* Convert to degrees */
    *h *= 60;
    if (*h < 0)
    {
        h += 360;
    }

    /* CALCULATE THE SATURATION */
    if (max == 0)
    {
        *s = 0;
    }
    else
    {
        *s = (delta / max);
    }

    /* CALCULATE THE VALUE (a.k.a brightness) */
    *v = max;
}

/*
    Algorithm adapted from:
    www.rapidtables.com/convert/hsv-to-rgb.htm
    www.cs.rit.edu/~ncs/color/t_convert.html
*/
void HSV_to_RGB(float *h, float *s, float *v, Uint8 *r, Uint8 *g, Uint8 *b)
{
    // printf("before convert back r: %d, g: %d, b: %d\n", *r, *g, *b);

    float r_float = 0.0;
    float g_float = 0.0;
    float b_float = 0.0;

    float c = (*v) * (*s);
    float x = c * (1 - (fmodf( (*h) / 60.0, 2) - 1));
    float m = (*v) - c;

    // printf("variables: c: %f, x: %f, m: %f\n", c, x, m);

    if ((*h >= 0) && (*h < 60))
    {
        r_float = c;
        g_float = x;
        b_float = 0;
    }
    else if ((*h >= 60) && (*h < 120))
    {
        r_float = x;
        g_float = c;
        b_float = 0;
    }
    else if ((*h >= 120) && (*h < 180))
    {
        r_float = 0;
        g_float = c;
        b_float = x;
    }
    else if ((*h >= 180) && (*h < 240))
    {
        r_float = 0;
        g_float = x;
        b_float = c;
    }
    else if ((*h >= 240) && (*h < 300))
    {
        r_float = x;
        g_float = 0;
        b_float = c;
    }
    else if ((*h >= 300) && (*h < 360))
    {
        r_float = c;
        g_float = 0;
        b_float = x;
    }

    *r = (r_float + m) * 255;
    *g = (g_float + m) * 255;
    *b = (b_float + m) * 255;

    // printf("after convert back r: %d, g: %d, b: %d\n", *r, *g, *b);
}

void change_brightness(Uint8 *r, Uint8 *g, Uint8 *b)
{

    /* Factor to increase the brightness by. 1->255 */
    int factor = 0;
    float ratio = ((float)M_BRIGHTNESS - 50.0) / 50.0;
    // printf("ratio: %f\n", ratio);
    factor = (int) (ratio * 255);

    /* Modify red */
    if ((*r + factor) > 255)
    {
        *r = 255;
    }
    else if ((*r + factor) < 0)
    {
        *r = 0;
    }
    else
    {
        *r += factor;
    }

    /* Modify green */
    if ((*g + factor) > 255)
    {
        *g = 255;
    }
    else if ((*g + factor) < 0)
    {
        *g = 0;
    }
    else
    {
        *g += factor;
    }

    /* Modify blue */
    if ((*b + factor) > 255)
    {
        *b = 255;
    }
    else if ((*b + factor) < 0)
    {
        *b = 0;
    }
    else
    {
        *b += factor;
    }
}

void manipulate_image(Uint8 *r, Uint8 *g, Uint8 *b)
{
    float h = 0;
    float s = 0;
    float v = 0;

    // printf("original rgb: r: %d, g: %d, b: %d\n", *r, *g, *b);

    /* Convert RGB to HSB */
    RGB_to_HSV(*r, *g, *b, &h, &s, &v);
    // printf("original hsv: h: %f, s: %f, v: %f\n", h, s, v);

    /* Change brightness if we need to */
    if (M_BRIGHTNESS != 50)
    {
        change_brightness(&(*r), &(*g), &(*b));
    }
    // printf("updated hsv: h: %f, s: %f, v: %f\n", h, s, v);

    /* Change contrast if we need to */

    /* Change saturation if we need to */


     // Convert HSB back to RGB
    // HSV_to_RGB(&h, &s, &v, &(*r), &(*g), &(*b));
    // printf("updated rgb: r: %d, g: %d, b: %d\n", *r, *g, *b);
    // exit(1);

}

// TODO MOVE THIS BACK IN THE RIGHT POSITION
/* Read frame data and write it to the screen */
int display_frame()
{
    /* Variables for each channel */
    Uint8 red_channel = 0;
    Uint8 green_channel = 0;
    Uint8 blue_channel = 0;

    /* Iterate across all pixels on the screen */
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            /* Create a pointer to the next pixel on the surface */
            int *p = SCREEN_SURFACE->pixels + y * SCREEN_SURFACE->pitch + x * SCREEN_SURFACE->format->BytesPerPixel;

            /* Read in the values from the ppm stream */
            set_next_pixel_RGB(&red_channel, &green_channel, &blue_channel);

            /* Perform image manipulation if we need to i.e. if any modifier is not 50 */
            if ((M_BRIGHTNESS != 50) || (M_CONTRAST != 50) || (M_SATURATION != 50))
            {
                manipulate_image(&red_channel, &green_channel, &blue_channel);
            }

            // printf("FINAL rgb: r: %d, g: %d, b: %d\n", red_channel, green_channel, blue_channel);
            // exit(1);

            /* Draw the pixel with the correct colours */
            *p=SDL_MapRGB(SCREEN_SURFACE->format, red_channel, green_channel, blue_channel);
        }
    }

    return 0; // success!
}

/* Primary function to play the video */
int video_player_init(int argc, char **argv)
{
    /* Set the values for colour manipulation */
    for (int i = 2; i < argc; i++)
    {
        /* Note: These atoi() conversions are safe since arg_parser has performed validation */
        switch(i)
        {
            case 2:
                M_BRIGHTNESS = atoi(argv[i]);
                break;
            case 3:
                M_CONTRAST = atoi(argv[i]);
                break;
            case 4:
                M_SATURATION = atoi(argv[i]);
                break;
            default:
                break;
        }
    }

    int delay_ms = atoi(argv[1]);
    printf("Playing video %dms delay.\n", delay_ms);
    printf("Image manipulation values:\n\tBrightness: %d\n\tContrast: %d\n\tSaturation: %d\n", M_BRIGHTNESS, M_CONTRAST, M_SATURATION);

    /* Peek the dimensions from the first frame header. Return -1 for invalid header. */
    if (peek_screen_dimensions() != 0)
    {
        printf("Error: Tried to peek header with invalid values.");
        return -1;
    }

    /* Initialise SDL and check it initialised correctly */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    else
    {
        /* Create the video player window using the dimensions */
        WINDOW = SDL_CreateWindow("ppmplayer",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        /* Check that the window was properly initialised */
        if (WINDOW == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return -1;
        }
        else
        {
            /* Create the screen. Default screen colour is purple. */
            SCREEN_SURFACE = SDL_GetWindowSurface(WINDOW);
            SDL_FillRect(SCREEN_SURFACE, NULL, SDL_MapRGB(SCREEN_SURFACE->format, 0xff, 0x00, 0xff));

            /* Variables for the frame update event loop */
            SDL_Event event;
            void (*fptr)(void *);
            SDL_TimerID frame_update_timer = SDL_AddTimer(delay_ms, timer_callback, NULL);

            /* Setup the SDL wait event handler loop. Run until there are no frames left or EOF. */
            while (peek_next_char() == 0 && feof(stdin) == 0 && SDL_WaitEvent(&event)) {
                switch(event.type) {
                    /* Program is manually quit by the user */
                    case SDL_QUIT:
                        printf("Closing video\n");
                        SDL_RemoveTimer(frame_update_timer);
                        SDL_Quit();
                        return -1;

                    /* The frame update event */
                    case SDL_USEREVENT:
                        fptr = event.user.data1;
                        void *arg = event.user.data2;
                        fptr(arg);

                    /* Ignore all other event types */
                    default:
                        break;
                }
            }
            /* Free memory used for the window surface */
            SDL_FreeSurface(SCREEN_SURFACE);
        }
        /* Free memory used for the window */
        SDL_DestroyWindow(WINDOW);
    }
    /* Quit SDL */
    SDL_Quit();
    return 0; // Success!
}

/* Read the dimensions from the first frame header and use these for the SDL_Window */
int peek_screen_dimensions()
{
    /* Variables */
    int peek_char = '\0';
    int magic_number_count = 0;
    int width_count = 0;
    int height_count = 0;

    /* Support maximum of 5 digit numbers for width and height */
    int magic_number_max = 2;
    int width_digit_max = 5;
    int height_digit_max = 5;

    /* Our memory allocations for height and width should be enough */
    char *peek_buffer_1 = (char *) malloc((magic_number_max+1)*sizeof(char));
    char *peek_buffer_2 = (char *) malloc((width_digit_max+1)*sizeof(char));
    char *peek_buffer_3 = (char *) malloc((height_digit_max+1)*sizeof(char));

    if ((peek_buffer_1 == NULL) || (peek_buffer_2 == NULL) || (peek_buffer_3 == NULL))
    {
        printf("Error: peek_screen_dimensions() - one or more peek_buffers is NULL\n");
        return -1;
    }

    /* Ensure memory allocations are correctly initialised */
    /* Using '\0' because its handy when using strlen() and atoi() - saves us from manually null terminating. */
    memset(peek_buffer_1, '\0', (magic_number_max+1));
    memset(peek_buffer_2, '\0', (width_digit_max+1));
    memset(peek_buffer_3, '\0', (height_digit_max+1));

    /* Peek the Magic Number and write to memory */
    while ((peek_char = fgetc(stdin)) != '\n')
    {
        peek_buffer_1[strlen(peek_buffer_1)] = peek_char;
        magic_number_count++;
    }

    /* Check Magic Number length */
    if (magic_number_count > magic_number_max)
    {
        printf("Error: peek_screen_dimensions() - Magic number is too long\n");
        return -1;
    }

    /* Peek the Width and write to memory */
    while ((peek_char = fgetc(stdin)) != ' ')
    {
        peek_buffer_2[strlen(peek_buffer_2)] = peek_char;
        width_count++;
    }

    /* Check the number of digits in the width */
    if (width_count > width_digit_max)
    {
        printf("Error: peek_screen_dimensions() - Width has %d digits. Program supports up to and including 5 digits.\n", width_count);
        return -1;
    }

    /* Peek the Height and write to memory */
    while ((peek_char = fgetc(stdin)) != '\n')
    {
        peek_buffer_3[strlen(peek_buffer_3)] = peek_char;
        height_count++;
    }

    /* Check the number of digits in the height */
    if (height_count > height_digit_max)
    {
        printf("Error: peek_screen_dimensions() - Height has %d digits. Program supports up to and including 5 digits.\n", height_count);
        return -1;
    }

    /* Convert the character arrays to proper int dimensions */
    int read_width = atoi(peek_buffer_2);
    int read_height = atoi(peek_buffer_3);

    /* Check the dimension values */
    if ((read_width <= 0) || (read_height <= 0))
    {
        printf("Error: peek_screen_dimensions() - Invalid dimension values. Width: %d, Height: %d\n", read_width, read_height);
        return -1;
    }

    /* Set the screen dimensions */
    SCREEN_WIDTH = read_width;
    SCREEN_HEIGHT = read_height;
    printf("Frame dimensions - Width: %d, Height %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Unpeek the characters in reverse order so that stdin is back to the start */

    /* Height */
    ungetc('\n', stdin);
    for (int i = strlen(peek_buffer_3); i > 0; i--)
    {
        ungetc(peek_buffer_3[i-1], stdin);
    }

    /* Width */
    ungetc(' ', stdin);
    for (int i = strlen(peek_buffer_2); i > 0; i--)
    {
        ungetc(peek_buffer_2[i-1], stdin);
    }

    /* Magic Number */
    ungetc('\n', stdin);
    for (int i = strlen(peek_buffer_1); i > 0; i--)
    {
        ungetc(peek_buffer_1[i-1], stdin);
    }

    /* Free our memory */
    free(peek_buffer_1);
    free(peek_buffer_2);
    free(peek_buffer_3);

    return 0; // success!
}

/* The timer callback function that points to the frame_update_loop() */
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

/* Core update loop */
void frame_update_loop(void *arg)
{
    /* Process the frame header */
    read_ppm_header();

    /* Read frame data and write to the screen */
    display_frame();

    /* Update the window to show the current screen */
    SDL_UpdateWindowSurface(WINDOW);

    /* Remove the -1 integer separator between frames */
    remove_frame_separator();
}

int check_for_comment_line()
{
    char c = 0;

    /* We have found a comment line */
    if ((c = fgetc(stdin)) == '#')
    {
        /* Read in the entire comment line */
        while ((c = fgetc(stdin)) != '\n')
        {
            // do nothing important
        }

        /* There is possibly another comment line after this */
        return 1;
    }

    /* There was no comment line, so put back the character */
    ungetc(c, stdin);
    return 0;
}

/* Read and process the frame header */
int read_ppm_header()
{
    /* Variables */
    char *line_buffer = NULL;
    int line_buffer_size = 0;

    /* Allocate memory for the Magic Number Line */
    line_buffer_size = 3;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: read_ppm_header() - line_buffer[magic_number] is NULL.\n");
        return -1;
    }

    /* Check if the frame is in valid ppm format i.e. P6 */
    if (check_valid_ppm(line_buffer, line_buffer_size) != 0)
    {
        return -1;
    }

    /* Free our memory */
    free(line_buffer);

    /* Check for comments */
    // while (check_for_comment_line() == 1)
    // {
    //     printf("Removing comment line.\n");
    // }

    /* Allocate memory for the width and the height. */
    line_buffer_size = 6;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: read_ppm_header() - line_buffer[dimension] is NULL.\n");
        return -1;
    }

    /* Check width - this function sets array values to zero first */
    if (check_width(line_buffer, line_buffer_size) != 0)
    {
        printf("Error: read_ppm_header() - invalid width.\n");
        return -1;
    }

    /* Check height - this function sets array values to zero first */
    if (check_height(line_buffer, line_buffer_size) != 0)
    {
        printf("Error: read_ppm_header() - invalid height.\n");
        return -1;
    }

    /* Free our memory */
    free(line_buffer);

    /* Check for comments */
    // while (check_for_comment_line() == 1)
    // {
    //     printf("Removing comment line.\n");
    // }

    /* Allocate memory for the max value - we are expecting 255. */
    line_buffer_size = 4;
    line_buffer = (char *) malloc(line_buffer_size * sizeof(char));

    if (line_buffer == NULL)
    {
        printf("Error: read_ppm_header() - line_buffer[max_val] is NULL.\n");
        return -1;
    }

    /* Check the max value in the header is valid */
    if (check_valid_max_val(line_buffer, line_buffer_size) != 0)
    {
        printf("Error: read_ppm_header() - invalid max val.\n");
        return -1;
    }

    /* Free our memory */
    free(line_buffer);

    /* Check for comments */
    // while (check_for_comment_line() == 1)
    // {
    //     printf("Removing comment line.\n");
    // }

    return 0; // success!
}

/* Validate the ppm frame format */
int check_valid_ppm(char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    int i = 0;
    char next_c = '\0';

    /* Read in the magic number */
    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    /* Null terminate array */
    line_buffer[i] = '\0';

    /* Fail if magic number is not 'P6' */   
    if (strcmp(line_buffer, "P6") != 0)
    {
        printf("Error: check_valid_ppm() - Not a valid PPM file! Magic number: %s\n. Expected values: [P6]\n", line_buffer);
        return -1;
    }

    return 0; // success!
}

/* Validate the frame width */
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

    /* Check that the value we read in is what we expect */
    if (atoi(line_buffer) != SCREEN_WIDTH)
    {
        printf("Error: check_width() - width has changed.\n");
        return -1;
    }
    
    return 0; //success!
}

/* Validate the frame height */
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

    /* Check that the value we read in is what we expect */
    if (atoi(line_buffer) != SCREEN_HEIGHT)
    {
        printf("Error: check_height() - height has changed.\n");
        return -1;
    }
    
    return 0; // success!
}

/* Validate the frame max value */
int check_valid_max_val(char *line_buffer, int line_buffer_size)
{
    /* Zero-initialize line_buffer */
    memset(line_buffer, 0, line_buffer_size);

    int i = 0;
    char next_c = '\0';

    /* Read in the max value */
    while ((next_c = fgetc(stdin)) != '\n')
    {
        line_buffer[i] = next_c;
        i++;
    }

    line_buffer[i] = '\0';

    /* Fail if max value is not what we expect i.e. 255 */   
    if (strcmp(line_buffer, "255") != 0)
    {
        printf("Error: check_valid_max_val() - Read: %s\n. Expected values: [255]\n", line_buffer);
        return -1;
    }

    return 0; // success!
}

void set_next_pixel_RGB(Uint8 *red_channel, Uint8 *green_channel, Uint8 *blue_channel)
{
    *red_channel = fgetc(stdin);
    *green_channel = fgetc(stdin);
    *blue_channel = fgetc(stdin);
}

// INSERT THE FUNCTION HERE

/* Remove the -1 integer separator betwen each frame */
void remove_frame_separator()
{
    /* Supports different systems that use different integer sizes */
    int num_bytes = sizeof(int);
    for (int i = 0; i < num_bytes; i++)
    {
        fgetc(stdin);
    }
}

/* Check if there is maybe another ppm frame in stdin */
int peek_next_char()
{
    /* Peek next character */
    int peek = 0;

    /* 80 == 'P' which is the start of a new frame */
    if ((peek = fgetc(stdin)) != 80)
    {
        printf("No more frames to read!\n");
        return -1;
    }

    /* Place the character back into stdin */
    ungetc(peek, stdin);

    return 0; // success!
}

// int i = 0;
    // float f = 0.0;
    // float p = 0.0;
    // float q = 0.0;
    // float t = 0.0;

    // /* If the image is achromatic (i.e. grey) */
    // if (*s == 0)
    // {
    //     *r = (Uint8)*v;
    //     *g = (Uint8)*v;
    //     *b = (Uint8)*v;
    //     // return; // skip processing!
    // }

    // *h /= 60.0;
    // i = (int)floorf(*h);
    // printf("i: %d\n", i);
    // f = *h - i;
    // p = *v * (1 - *s);
    // q = *v * (1 - *s * f);
    // t = *v * (1 - *s * (1 - f));

    // switch(i)
    // {
    //     case 0:
    //         r_float = *v;
    //         g_float = t;
    //         b_float = p;
    //         break;
    //     case 1:
    //         r_float = q;
    //         g_float = *v;
    //         b_float = p;
    //         break;
    //     case 2:
    //         r_float = p;
    //         g_float = *v;
    //         b_float = t;
    //         break;
    //     case 3:
    //         r_float = p;
    //         g_float = q;
    //         b_float = *v;
    //         break;
    //     case 4:
    //         r_float = t;
    //         g_float = p;
    //         b_float = *v;
    //         break;
    //     default:
    //         r_float = *v;
    //         g_float = p;
    //         b_float = q;
    //         break;
    // }