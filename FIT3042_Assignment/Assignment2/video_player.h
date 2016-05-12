#include <stdint.h>
typedef uint32_t Uint32;
typedef uint8_t Uint8;

/******************************************************************************
* Name    : video_player_init
* Desc    : Primary function that plays the video from the std input using SDL.
*           Sets up and destroy required SDL components. SDL wait event loop is
*           created and run from here.
*
*			Also sets the flags for image manipulation based on the command
*			line arguments.
*
* Params  : argc - the number of command line arguments passed in.
*			argv - an array of the command line arguments.
*           
* Return  : 0 on success, -1 on failure or SDL_QUIT interrupt.
******************************************************************************/
int video_player_init(int argc, char **argv);

/******************************************************************************
* Name    : peek_screen_dimensions
* Desc    : Peeks the header of the initial input frame in order to read in the
*           width and height to setup the SDL_Window.
*
*           Uses fgetc() to read in chars from stdin, saves these chars to memory,
*           and then puts them back to stdin in reverse order using ungetc().
*
*           Also checks the frame header is valid ppm format.
*
* Params  : 
*           
* Return  : 0 on success, -1 on failure.
******************************************************************************/
int peek_screen_dimensions();

/******************************************************************************
* Name    : timer_callback
* Desc    : Callback function that is set to run every update loop. Sets up the
*           relevant SDL events and points to the frame_update_loop() function
*           with a pointer.
*
* Params  : interval - Set to the first command line argument. Determines how
*                      often the callback is run in ms.
*           
* Return  : The interval between each call in ms.
******************************************************************************/
Uint32 timer_callback(Uint32 interval, void *param);

/******************************************************************************
* Name    : frame_update_loop
* Desc    : Peeks the header of the initial input frame in order to read in the
*           width and height to setup the SDL_Window. Uses fgetc() to read in
*           chars from stdin, saves these chars to memory, and then puts them
*           back to stdin in reverse order using ungetc().
*
*           Also checks the frame header is valid ppm format.
*
* Params  : params - Pointer to the parameters passed to this
*                    function from timer_callback().
*           
* Return  : None.
******************************************************************************/
void frame_update_loop(void *params);

/******************************************************************************
* Name    : read_ppm_header
* Desc    : Function that reads the ppm frame header from the stdin stream.
*           Performs checking and validation on the values that are read in.
*
* Params  : None.
*           
* Return  : 0 on success, -1 on when invalid values are read.
******************************************************************************/
int read_ppm_header();

/******************************************************************************
* Name    : check_valid_ppm
* Desc    : Function that reads the ppm magic number and checks that it is "P6"
*
* Params  : None.
*           
* Return  : 0 when "P6" is read, -1 otherwise.
******************************************************************************/
int check_valid_ppm(char *line_buffer, int line_buffer_size);

/******************************************************************************
* Name    : check_width
* Desc    : Function that reads the frame width and checks if it is the same
*           as originally set.
*
* Params  : None.
*           
* Return  : 0 when the width matches SCREEN_WIDTH, -1 otherwise.
******************************************************************************/
int check_width(char *line_buffer, int line_buffer_size);

/******************************************************************************
* Name    : check_height
* Desc    : Function that reads the frame height and checks if it is the same
*           as originally set.
*
* Params  : None.
*           
* Return  : 0 when the width matches SCREEN_HEIGHT, -1 otherwise.
******************************************************************************/
int check_height(char *line_buffer, int line_buffer_size);

/******************************************************************************
* Name    : check_valid_max_val
* Desc    : Function that reads the max value from the ppm frame header, and
*           checks if it is valid or not. This program only supports 255, so
*           this is enforced.
*
* Params  : None.
*           
* Return  : 0 when max val is valid, -1 otherwise.
******************************************************************************/
int check_valid_max_val(char *line_buffer, int line_buffer_size);

/******************************************************************************
* Name    : set_next_pixel_RGB
* Desc    : Function that sets the RGB colour values of the next pixel from the
*			stdin stream.
*
* Params  : red_channel - pointer to where the red_channel variable is stored.
*			green_channel - pointer to where the green_channel variable is stored.
*			blue_channel - pointer to where the blue_channel variable is stored.
*           
* Return  : None.
******************************************************************************/
void set_next_pixel_RGB(Uint8 *red_channel, Uint8 *green_channel, Uint8 *blue_channel);

/******************************************************************************
* Name    : display_frame
* Desc    : Function that iterates across the number of expected pixels (i.e.
*           SCREEN_WIDTH * SCREEN_HEIGHT), reads the next R, G, and B colour
*           values from stdin, and then writes a pixel to the current position.
*
* Params  : None.
*           
* Return  : 0 when all pixels are written, -1 otherwise.
******************************************************************************/
int display_frame();

/******************************************************************************
* Name    : display_frame
* Desc    : Function that removes the -1 integer separater between each frame.
*           Expected integer is 4 bytes, but support is included for integers
*           of two bytes. Uses fgetc() to take the individual bytes from stdin.
*
* Params  : None.
*           
* Return  : None.
******************************************************************************/
void remove_frame_separator();

/******************************************************************************
* Name    : display_frame
* Desc    : Function that peeks at the next char in stdin to see if a new frame
*           exists or not. If the char is "P", then another frame may exist.
*
* Params  : None.
*           
* Return  : 0 if "P" is the next char, -1 otherwise.
******************************************************************************/
int peek_next_char();