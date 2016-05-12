#include <math.h>
#include "image_manipulator.h"

extern int M_BRIGHTNESS;
extern int M_CONTRAST;
extern int M_SATURATION;

void manipulate_image(Uint8 *r, Uint8 *g, Uint8 *b)
{
    /* HSV variables */
    float h = 0;
    float s = 0;
    float v = 0;

    /* Convert RGB to HSB */
    RGB_to_HSV(*r, *g, *b, &h, &s, &v);

    /* Change brightness if we need to */
    if (M_BRIGHTNESS != 50)
    {
        change_brightness(&(*r), &(*g), &(*b));
        /* Note: we don't use HSV so don't need to convert back */
    }

    /* Change contrast if we need to */
    if (M_CONTRAST != 50)
    {
        change_contrast(&v);
        HSV_to_RGB(&h, &s, &v, &(*r), &(*g), &(*b));
    }

    /* Change saturation if we need to */
    if (M_SATURATION != 50)
    {
        change_saturation(&s);
        HSV_to_RGB(&h, &s, &v, &(*r), &(*g), &(*b));
    }   
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
    www.rapidtables.com/convert/hsv-to-rgb.htm
    www.cs.rit.edu/~ncs/color/t_convert.html
*/
void HSV_to_RGB(float *h, float *s, float *v, Uint8 *r, Uint8 *g, Uint8 *b)
{

    float r_float = 0.0;
    float g_float = 0.0;
    float b_float = 0.0;

    float c = (*v) * (*s);
    float x = c * (1 - (fmodf( (*h) / 60.0, 2) - 1));
    float m = (*v) - c;

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
}

void change_brightness(Uint8 *r, Uint8 *g, Uint8 *b)
{
    /* Factor to increase the brightness by */
    int factor = 0;
    float ratio = ((float)M_BRIGHTNESS - 50.0) / 50.0;
    factor = (int) (ratio * 255);

    /* Modify red and ensure it is in range */
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

    /* Modify green and ensure it is in range */
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

    /* Modify blue and ensure it is in range */
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

void change_contrast(float *v)
{
    /* Set the factor to change the contrast */
    float factor = ((float)M_CONTRAST - 50.0) / 50.0;

    *v += factor;

    /* *v must be between 0.0 and 1.0 */
    if (*v > 1.0)
    {
        *v = 1.0;
    }
    else if (*v < 0.0)
    {
        *v = 0.0;
    }
}

void change_saturation(float *s)
{
    /* Set the factor to change the saturation */
    float factor = ((float)M_SATURATION - 50.0) / 50.0;
    
    *s += factor;

    /* *s must be between 0.0 and 1.0 */
    if (*s > 1.0)
    {
        *s = 1.0;
    }
    else if (*s < 0.0)
    {
        *s = 0.0;
    }
}