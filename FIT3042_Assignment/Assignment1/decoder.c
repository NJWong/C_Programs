#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "decoder.h"

// TODO move to header file
void scale_frame_data(unsigned char **scaled_data, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data, int scalefactor, int image_pixels, int width, int height);

/************************************************************
*                      RLE FILE DECODING
************************************************************/

/************************************************************
* Desc   : Function that decodes an input .rle file and
*          either writes the output to stdout, or writes the
*          output for each frame to a separate .ppm file.
*
* Params : argv - an array of the command line arguments
*           
* Return : None
*
* Result : Input rle file is decoded and output using the
*          specified output method. If 'scale' or 'tween' is
*          applied, then the output data will reflect this.
************************************************************/
void decode_rlefile(char **argv, int num_of_mods)
{
    FILE *rlefile = fopen(argv[1], "rb");
    if (rlefile == NULL)
    {
        fprintf(stderr, "*rlefile is a null pointer. Exiting.\n");
        return;
    }

    int scale_mod = (num_of_mods > 0 && (strcmp(argv[3], "--scale") == 0)); // TODO handle for argv[5] as well
    /* Variables for modifications - initialised to default values */
    int scalefactor = 1;
    // int tweenfactor = 0;

    /* Variables for image decompression */
    int *dimensions = NULL;
    int width;
    int height;
    int image_pixels;

    /* Variables for data output */
    char *prefix = argv[2];
    int to_stdout = (strcmp(argv[2], "-") == 0);
    unsigned char *red_frame_data = NULL;
    unsigned char *green_frame_data = NULL;
    unsigned char *blue_frame_data = NULL;
    unsigned char *key_frame_data = NULL;

    /* Determine the size of the image in the rle video file */
    dimensions = (int *) malloc(2*sizeof(int));
    if (dimensions == NULL)
    {
        fprintf(stderr, "*dimensions is a null pointer. Exiting.\n");
        return;
    }
    get_dimensions(rlefile, dimensions);

    width = dimensions[0];
    height = dimensions[1];
    image_pixels = width * height;
    fprintf(stderr, "width: %d, height: %d, pixels:%d\n", width, height, image_pixels);

    if (scale_mod)
    {
        scalefactor = atoi(argv[4]);
        printf("scale: %d\n", scalefactor);

        /* Scale the total number of pixels that we need to 'malloc' for */
    }

    /* Initialise the arrays to store decompressed frame data */
    key_frame_data = (unsigned char *) malloc((image_pixels * 3) * sizeof(unsigned char));

    red_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    green_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    blue_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));

    /* Check if these array pointers are not NULL */
    if (key_frame_data == NULL || red_frame_data == NULL || green_frame_data == NULL || blue_frame_data == NULL)
    {
        fprintf(stderr, "Pointer to one or more data storage arrays are NULL. Exiting.\n");
        return;
    }

    /* Read characters from the next frame until we each the end of the file */
    char c = '\0';
    int frame_counter = 1;

    /* Note: Each key frame begins with 'K'. This should not be decoded so is removed each loop by the 'fgetc()' call */
    while ((c = fgetc(rlefile)) != EOF)
    {
        /* An empty rle file only has an 'E', so check for that */
        if (c != 'E')
        {
            /* We've found the next key frame */
            decompress_and_store_key_frame_data(rlefile, key_frame_data, image_pixels);

            /* Copy separate channel values into their own arrays */
            separate_channel_values(key_frame_data, red_frame_data, green_frame_data, blue_frame_data, image_pixels);

            /* If we are scaling, then scale the individual channel values */
            if (scale_mod)
            {
                /* Note: '--scale 1' in effect does nothing */
                if (scalefactor != 1)
                {
                    /* Allocate for width */
                    unsigned char **scaled_data = (unsigned char **) malloc(((scalefactor * (width - 1)) + 1) * sizeof(unsigned char*));

                    /* Allocate for height */
                    for (int row = 0; row < ((scalefactor * (width - 1)) + 1); row++)
                    {
                        scaled_data[row] = (unsigned char *) malloc(((scalefactor * (height - 1)) + 1) * sizeof(unsigned char));
                    }

                    /* Scale the decoded image to the specified scalefactor */
                    scale_frame_data(scaled_data, red_frame_data, green_frame_data, blue_frame_data, scalefactor, image_pixels, width, height);

                    /* Send scaled decoded data to specified output stream*/
                    printf("frame sent to output\n");

                    free(scaled_data);
                }
                
            }
            else
            {
                /* Output using the method specified in argv[2] */
                if (to_stdout)
                {
                    /* Send decoded data to stdout */
                    send_frame_to_stdout(width, height, red_frame_data, green_frame_data, blue_frame_data);

                    /* Separate each frame with the integer -1 */
                    int frame_separator = -1;
                    /* Note: printf() outputs to the stdout stream */
                    printf("%d\n", frame_separator);   
                }
                else
                {
                    /* Send decoded data to a ppm file */
                    send_frame_to_ppm(width, height, red_frame_data, green_frame_data, blue_frame_data, prefix, frame_counter);
                    frame_counter++;
                }
            }
        }
    }

    /* Cleanup */
    free(dimensions);
    free(key_frame_data);
    free(red_frame_data);
    free(green_frame_data);
    free(blue_frame_data);
    
    fclose(rlefile);
}

void scale_frame_data(unsigned char **scaled_data, unsigned char *red_frame_data, unsigned char *green_frame_data,
                      unsigned char *blue_frame_data, int scalefactor, int image_pixels, int width, int height)
{
    
    printf("Normal frame data\n");

    int test_width = 3;
    int test_height = 3;

    int **test = malloc(test_width * sizeof(int *));
    for (int i = 0; i < test_width; i++)
    {
        test[i] = malloc(test_height * sizeof(int));
    }

    int counter = 0;
    for (int h = 0; h < test_height; h++)
    {
        for (int w = 0; w < test_width; w++)
        {
            test[w][h] = counter;
            counter++;
            printf("%d ", test[w][h]);
        }
        printf("\n");
    }

    printf("Scaled frame data\n");

    int scaled_width = ((scalefactor * (test_width - 1)) + 1);
    int scaled_height = ((scalefactor * (test_height - 1)) + 1);

    int **scaled = malloc(scaled_width * sizeof(int *));
    for (int i = 0; i < scaled_width; i++)
    {
        scaled[i] = malloc(scaled_height * sizeof(int));
    }

    // counter = 0;
    for (int h = 0; h < scaled_height; h++)
    {
        for (int w = 0; w < scaled_width; w++)
        {
            scaled[w][h] = 0;
            printf("%d ", scaled[w][h]);
        }
        printf("\n");
    }

    int test_x = 0;
    int test_y = 0;

    int scaled_x = (scalefactor * test_x) - 1;
    int scaled_y = (scalefactor * test_y) - 1;

    // for (int th = 0; th < test_height; th++)
    // {
    //     for (int tw = 0; tw < test_width; tw++)
    //     {
    //         int scaled_x = ((scalefactor * tw) + 1);
    //         int scaled_y = ((scalefactor * th) + 1);

    //         scaled[scaled_x][scaled_y] = test[tw][th];
    //     }
    // }

    // for (int h = 0; h < scaled_height; h++)
    // {
    //     for (int w = 0; w < scaled_width; w++)
    //     {
    //         printf("%d ", scaled[w][h]);
    //     }
    //     printf("\n");
    // }

    free(test);

    /* Scale the red channel data */

    
    /* Iterate over all of the original pixel values */
    // for (int h = 0; h < height; h++)
    // {
    //     for (int w = 0; w < width; w++)
    //     {
    //         int red_index = (width * (h - 1)) + w;
    //         tmp_data[copy_index] = red_frame_data[];
    //     }
    // }

    /* Scale the green channel data */


    /* Scale the blue channel data */
}

/************************************************************
* Desc   : Function that will read the width and height of the
*          images in the rle file from the <header> -> 
*          <properties> described in the BNF.
*
* Params : rlefile - the video file we want to decode
*          dimensions - an array that stores two int values
*           
* Return : None
*
* Result : Input rle file is decoded and output using the
*          specified output method. If 'scale' or 'tween' is
*          applied, then the output data will reflect this.
************************************************************/
void get_dimensions(FILE *rlefile, int *dimensions)
{
    /* Allocate space to store the width and height properties read from the rlefile. */
    /* It's reasonable to assume that neither width nor height will not exceed 5 digits. */
    char *width_string = (char *) malloc(5*sizeof(char));
    char *height_string = (char *) malloc(5*sizeof(char));

    /* Read in characters to get bypass the rle id-string */
    bypass_id_string(rlefile);

    /* Get the width in pixels and store in a char array */
    get_width(rlefile, width_string);

    /* Get the height in pixels and store in a char array */
    get_height(rlefile, height_string);

    /* Convert width_string and height_string to int values and store them in an array */
    dimensions[0] = atoi(width_string);
    dimensions[1] = atoi(height_string);

    /* Cleanup */
    free(width_string);
    free(height_string);
}

/************************************************************
* Desc   : Function that simply calls 'fgetc()' on the video
*          file until the <id_string> is passed. We do this
*          because we don't use "RLEPLAYv1" for decoding.
*
* Params : rlefile - the video file we want to decode
*           
* Return : None
************************************************************/
void bypass_id_string (FILE *rlefile)
{
    char next_char;

    /* We know "RLEPLAYv1" takes up the first line, so look for a new line */
    while((next_char = fgetc(rlefile)) != '\n')
    {
        continue;
    }
}

/************************************************************
* Desc   : Function that uses 'fgetc()' on the video file in
*          order to read out and save the characters that
*          indicate the image's width in pixels.
*
* Params : rlefile - the video file we want to decode
*          width_string - the string to which we save the width
*           
* Return : None
************************************************************/
void get_width(FILE *rlefile, char *width_string)
{
    char next_char;

    /* Width starts on the new line, and ends before a ' ' (space character) */
    while((next_char = fgetc(rlefile)) != ' ')
    {
        /* Essentially appends a character to the end of the string */
        /* strlen(width_string) is used as the index of where to put next_char */
        width_string[strlen(width_string)] = next_char;
    }
}

/************************************************************
* Desc   : Function that uses 'fgetc()' on the video file in
*          order to read out and save the characters that
*          indicate the image's height in pixels.
*
* Params : rlefile - the video file we want to decode
*          height_string - the string to which we save the height
*           
* Return : None
************************************************************/
void get_height(FILE *rlefile, char *height_string)
{
    char next_char;
    
    /* Width starts after the ' ' (space character), and ends before the next '\n' character */
    while((next_char = fgetc(rlefile)) != '\n')
    {
        /* Essentially appends a character to the end of the string */
        /* strlen(height_string) is used as the index of where to put next_char */
        height_string[strlen(height_string)] = next_char;
    }
}

/************************************************************
* Desc   : Function that decompresses the data for a frame in
*          a video file using a modified version of Michael
*          Dipperstein's PackBit's algorithm variant (found
*          at http://michael.dipperstein.com/rle/index.html).
*          Characters are read in one at a time and decoded
*          until all the values for a frame have been added
*          to an array for storage.
*
* Params : rlefile - the video file we want to decode
*          key_frame_data - the array where we store the decoded values
*          height - the height of the image in pixels
*          width - the width of the image in pixels
*           
* Return : None
************************************************************/
void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data, int image_pixels)
{
    int countChar;
    int currChar;
    int value_counter = 0;
    int min_run_length = 3;

    /* Decode exactly the number of values required for a single frame */
    /* Note: We know the width and height, and that the R, G, and B values are stored
       separately, so the number of values required is 'image_pixels * 3'. */
    while (value_counter < (image_pixels * 3))
    {
        /* Read in the next char */
        countChar = fgetc(rlefile);

        /* Force it to have a sign */
        countChar = (char)countChar;

        if (countChar < 0)
        {
            /* Found a run, write out 2 - countChar copies */
            countChar = (min_run_length - 1) - countChar;

            if (EOF == (currChar = fgetc(rlefile)))
            {
                countChar = 0;
            }

            while (countChar > 0)
            {
                key_frame_data[value_counter] = currChar;
                value_counter++;
                countChar--;
            }
        }
        else
        {
            /* Found a block of countChar + 1 symbols to copy */
            for (countChar++; countChar > 0; countChar--)
            {
                if ((currChar = fgetc(rlefile)) != EOF)
                {
                    key_frame_data[value_counter] = currChar;
                    value_counter++;
                }
                else
                {
                    break;
                }
            }
        }
    }
    /* For debugging, show how many total values (i.e individual R, G, and B values) read for this frame */
    fprintf(stderr, "Total frame values: %d\n", value_counter);
}

/************************************************************
* Desc   : Function that separates the data stored in the key
*          frame data array into separate red, green, and blue
*          data arrays.
*
* Params : key_frame_data - the array where we store the decoded values
*          red_frame_data - the array for the red channel
*          green_frame_data - the array for the green channel
*          blue_frame_data - the array for the blue channel
*          image_pixels - the total number of pixels in a frame
*           
* Return : None
************************************************************/
void separate_channel_values(unsigned char *key_frame_data,
                             unsigned char *red_frame_data,
                             unsigned char *green_frame_data,
                             unsigned char *blue_frame_data,
                             int image_pixels)
{
    int r_index = 0;
    int g_index = 0;
    int b_index = 0;

    /* Note: key_frame_data consists of data as follows: [R, R, R, ...., G, G, G, ..., B, B, B] 
       The last R value is at index 'image_pixels - 1',
       the last G value is at index '2*image_pixels - 1'
       and the rest of the array is B values.*/
    for (int i = 0; i < (image_pixels * 3); i++)
    {
        if (i < image_pixels)
        {
            /* Write to the red channel array */
            red_frame_data[r_index] = key_frame_data[i];
            r_index++;
        }
        else if (i < (image_pixels * 2))
        {
            /* Write to the green channel array */
            green_frame_data[g_index] = key_frame_data[i];
            g_index++;
        }
        else if (i < (image_pixels * 3))
        {
            /* Write to the blue channel array */
            blue_frame_data[b_index] = key_frame_data[i];
            b_index++;
        }
    }
}

/************************************************************
* Desc   : Function that sends the decoded data to a .ppm file.
*
* Params : width - the width of the frame in pixels
*          height - the height of the frame in pixels
*          red_frame_data - the array for the red channel
*          green_frame_data - the array for the green channel
*          blue_frame_data - the array for the blue channel
*          prefix - the string prefix for the .ppm file
*          frame_counter - the current frame number for the rle file
*          
* Return : None
************************************************************/
void send_frame_to_ppm(int width, int height,
                       unsigned char *red_frame_data,
                       unsigned char *green_frame_data,
                       unsigned char *blue_frame_data,
                       char *prefix,  int frame_counter)
{
    /* Assuming a filename will not be more than 50 characters */
    char *filename = (char *) malloc(50 * sizeof(char));

    /* Create a file with appropriate naming convention */
    sprintf(filename, "%s-%04d.ppm", prefix, frame_counter);
    FILE *outFile = fopen(filename, "w");

    /* Check it is not NULL */
    if (outFile == NULL)
    {
        fprintf(stderr, "Pointer to outfile is NULL. Exiting.\n");
        return;
    }

    /* Allocate space for the string with the width and height we want to
       write to the .ppm file. Reasonable to assume neither width nor height
       will be more than 4 digits */
    char *dimension_string = (char *) malloc(10 * sizeof(char));

    //TODO remember that 'width' and 'height' are the unscaled values

    sprintf(dimension_string, "%d %d", width, height);

    /* Write file header information to .ppm file */
    fputs("P6\n", outFile);
    fputs(dimension_string, outFile);
    fputs("\n255\n", outFile);

    /* Merge pixel R, G, and B data together as specified by the .ppm format */
    int pixel_index = 0;
    for (int h = 1; h <= height; h++)
    {
        for (int w = 1; w <= width; w++)
        {
            fputc(red_frame_data[pixel_index], outFile);
            fputc(green_frame_data[pixel_index], outFile);
            fputc(blue_frame_data[pixel_index], outFile);
            pixel_index++;
        }
    }

    fprintf(stderr, "Wrote frame #%d\n", frame_counter);

    /* Cleanup */
    free(filename);
    free(dimension_string);

    fclose(outFile);
}

/************************************************************
* Desc   : Function that sends the decoded data to stdout.
*
* Params : width - the width of the frame in pixels
*          height - the height of the frame in pixels
*          red_frame_data - the array for the red channel
*          green_frame_data - the array for the green channel
*          blue_frame_data - the array for the blue channel
*          
* Return : None
************************************************************/
void send_frame_to_stdout(int width, int height,
                          unsigned char *red_frame_data,
                          unsigned char *green_frame_data,
                          unsigned char *blue_frame_data)
{
    /* Stream out the .ppm header information */
    fprintf(stdout, "P6\n%d %d\n255\n", width, height);

    //TODO remember that 'width' and 'height' are the unscaled values

    /* Merge pixel R, G, and B data together as specified by the .ppm format */
    int pixel_index = 0;
    for (int h = 1; h <= height; h++)
    {
        for (int w = 1; w <= width; w++)
        {
            fprintf(stdout, "%c", red_frame_data[pixel_index]);
            fprintf(stdout, "%c", green_frame_data[pixel_index]);
            fprintf(stdout, "%c", blue_frame_data[pixel_index]);
            pixel_index++;
        }
    }
}