#include <stdint.h>
typedef uint32_t Uint32;
typedef uint8_t Uint8;

/******************************************************************************
* Name    : manipulate_image
* Desc    : Function that manipulates the RGB values of a pixel based on the
*			input from command line arguments.
*
* Params  : *r - reference to where the red_channel is stored from video_player.
*			*g - reference to where the green_channel is stored from video_player.
*			*b - reference to where the blue_channel is stored from video_player.
*           
* Return  : None.
******************************************************************************/
void manipulate_image(Uint8 *r, Uint8 *g, Uint8 *b);

/******************************************************************************
* Name    : RGB_to_HSV
* Desc    : Converts the RGB values to HSV values.
*
* Params  : r - value of the red channel.
*			g - value of the green channel.
*			b - value of the blue channel.
*			*h - reference to where the hue is stored.
*			*s - reference to where the saturation is stored
*			*v - reference to where the value is stored
*           
* Return  : None.
******************************************************************************/
void RGB_to_HSV(Uint8 r, Uint8 g, Uint8 b, float *h, float *s, float *v);

/******************************************************************************
* Name    : find_max
* Desc    : Find the highest value of three options.
*
* Params  : r - the value of the red channel
*			g - the value of the green channel
*			b - the value of the blue channel
*           
* Return  : The highest value.
******************************************************************************/
float find_max(float r, float g, float b);

/******************************************************************************
* Name    : find_min
* Desc    : Find the lowest value of three options.
*
* Params  : r - the value of the red channel
*			g - the value of the green channel
*			b - the value of the blue channel
*           
* Return  : The lowest value.
******************************************************************************/
float find_min(float r, float g, float b);

/******************************************************************************
* Name    : HSV_to_RGB
* Desc    : Converts the HSV values to RGB values.
*
* Params  : *h - reference to where the hue is stored.
*			*s - reference to where the saturation is stored
*			*v - reference to where the value is stored
*			r - reference of the red channel.
*			g - reference of the green channel.
*			b - reference of the blue channel.
*           
* Return  : None.
******************************************************************************/
void HSV_to_RGB(float *h, float *s, float *v, Uint8 *r, Uint8 *g, Uint8 *b);

/******************************************************************************
* Name    : change_brightness
* Desc    : Change the brightness of the pixel whose RGB value is given.
*			Converts the 0->100 command line argument to a positive or negative
*			integer between 0->255. This value is added to each channel to
*			increase/decrease the brightness.
*
* Params  : r - reference to the red channel
*			g - reference to the green channel
*			b - reference to the blue channel
*           
* Return  : None.
******************************************************************************/
void change_brightness(Uint8 *r, Uint8 *g, Uint8 *b);

/******************************************************************************
* Name    : change_contrast
* Desc    : Change the contrast of the pixle whose RGB value is given. Does this
*			by changing the value (in the HSV colour) by a factor based on the
*			second image manipulation command line argument.
*
* Params  : *v - reference to the value
*           
* Return  : None.
******************************************************************************/
void change_contrast(float *v);

/******************************************************************************
* Name    : change_saturation
* Desc    : Change the saturation of the pixle whose RGB value is given. Does this
*			by changing the saturation (in the HSV colour) by a factor based on the
*			third image manipulation command line argument.
*
* Params  : *s - reference to the saturation
*           
* Return  : None.
******************************************************************************/
void change_saturation(float *s);