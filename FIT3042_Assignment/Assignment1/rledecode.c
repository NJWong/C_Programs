#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rledecode.h"

/************************************************************
* Desc   : Function that decodes an input .rle file and
*          either writes the output to stdout, or writes the
*          output for each frame to a separate .ppm file.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure. The stderr output
*          stream is used for showing helpful messages to
*          the user.
************************************************************/
int rledecode(int argc, char **argv)
{
    fprintf(stderr, "\n--- Starting rledecode ---\n");

    /* Check that the command line arguments are valid */
    if (validate_args(argc, argv) != 0)
    {
        /* If invalid, then stop here and return to main */
        fprintf(stderr, "Args are not valid.\n");
        return -1;
    }

    /* Decode the .rle file and put to output defined in arg2 */
    if (strcmp(argv[2], "-") == 0)
    {
        decode_to_stdout(argv);
    }
    else
    {
        decode_to_ppm(argv);
    }

    return 0;
}

/************************************************************
* Desc   : Function that checks whether the command line
*          arguments are valid or not.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int validate_args(int argc, char **argv)
{
    int correct_number_of_args = check_number_of_args(argc);
    if (correct_number_of_args == -1)
    {
        return -1;
    }

    int arg1_is_valid = parse_arg(1, argv[1]);
    int arg2_is_valid = parse_arg(2, argv[2]);
    // int arg3_is_valid = parse_arg(3, argv[3]);
    // int arg4_is_valid = parse_arg(4, argv[4])

    /* Check argv[3] and argv[4] are not both scale or tween */
    // TODO

    /* Note: the '!' is there so that we return 0 when all args are valid */
    return !((arg1_is_valid == 0) && ((arg2_is_valid == 1) || (arg2_is_valid == 2)));
}

/************************************************************
* Desc   : Function that checks if there is the correct
*          number of arguments passed through the command line
*          Minimum is 3:
*          ./rledecode <*.rle> (<prefix> || "-")
*          Maximum is 7:
*          ./rledecode <*.rle> (<prefix> || "-") --scale <sf> --tween <tf>
*          4 or 6 is strictly disallowed
           i.e. argc can only be 3, 5, and 7
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int check_number_of_args(int argc)
{
    if (argc == 3 || argc == 5 || argc == 7)
    {
        return 0;
    }
    else {
        /* Note: (argc - 1) is used because argv[0] is './rledecode' */
        fprintf(stderr, "Incorrect number of arguments: %d.\n", (argc - 1));
        return -1;
    }
}

/************************************************************
* Desc   : Function that deligates which argument to handle
*          to the appropriate handler function.
*
* Params : arg_index - the 
*          argc - the number of given command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int parse_arg(int arg_index, char *arg)
{
    /* Flag that determines success or failure */
    int flag = -1;

    switch(arg_index) {
        case 1:
            flag = handle_arg1(arg);
            break;
        case 2:
            flag = handle_arg2(arg);
            break;
        case 3:
            flag = handle_arg3(arg);
            break;
        case 4:
            flag = handle_arg4(arg);
            break;
        default:
            fprintf(stderr, "Error: parse_argument - incorrect arg_index");
            break;
    }

    return flag;
}

/************************************************************
* Desc   : Function that determines if argv[1] is valid or
*          not by checking if that rle file exists. Output
*          messages are put to the stderr stream for the
*          user to see. This keeps the stdout stream clean.
*
* Params : filepath - relative path to the rle file
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int handle_arg1(char *filepath)
{
    /* Handle empty string and NULL pointer */
    if (strcmp(filepath, "") == 0 || filepath == NULL) {
        return -1;
    }

    fprintf(stderr, "Looking for %s...\n", filepath);

    /* Determine if the rle file exists or not */
    if (rleplay_file_exists(filepath))
    {
        fprintf(stderr, "%s exists. Proceeding...\n", filepath);
    }
    else
    {
        fprintf(stderr, "%s does not exist.\n", filepath);
        return -1;
    }    

    return 0;
}

/************************************************************
* Desc   : Function that determines if argv[1] is valid or
*          not by checking if that rle file exists. Output
*          messages are put to the stderr stream for the
*          user to see. This keeps the stdout stream clean.
*
* Params : filepath - relative path to the rle file
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int rleplay_file_exists(char *filepath)
{
    struct stat buffer;
    if (filepath != NULL)
    {
        return (stat (filepath, &buffer) >= 0); // stat returns a negative value on failure
    }
    return -1; // return as if stat had failed
}

/* Handler and methods for argv[2] */
int handle_arg2(char *arg2)
{
    if (strcmp(arg2, "-") == 0)
    {
        return 1;
    }
    else if (is_valid_prefix(arg2))
    {
        return 2;
    }
    else
    {
        fprintf(stderr, "Invalid value for arg2\n");
        return -1;
    }

    return 0;
}

int is_valid_prefix(char *prefix)
{
    return (strlen(prefix) > 0);
}

/* Handler and methods for argv[3] */
int handle_arg3(char *arg3)
{
    fprintf(stderr, "handle_arg3");
    return 0;
}

/* Handler and methods for argv[4] */
int handle_arg4(char *arg4)
{
    fprintf(stderr, "handle_arg4");
    return 0;
}




void bypass_id_string (FILE *rlefile)
{
    char next_char;

    while((next_char = fgetc(rlefile)) != '\n')
    {
        continue;
    }
}

void get_width(FILE *rlefile, char *width_string)
{
    char next_char;

    while((next_char = fgetc(rlefile)) != ' ')
    {
        width_string[strlen(width_string)] = next_char; // TODO explain this
    }
}

void get_height(FILE *rlefile, char *height_string)
{
    char next_char;
    
    while((next_char = fgetc(rlefile)) != '\n')
    {
        height_string[strlen(height_string)] = next_char; // TODO explain this
    }
}

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

/* Send one frame's worth of data to stdout */
void send_frame_to_stdout(int width, int height,
                          unsigned char *red_frame_data,
                          unsigned char *green_frame_data,
                          unsigned char *blue_frame_data)
{
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

/* Write one frame's worth of data to a ppm file */
void send_frame_to_ppm(int width, int height,
                       unsigned char *red_frame_data,
                       unsigned char *green_frame_data,
                       unsigned char *blue_frame_data,
                       char *prefix,  int frame_counter)
{
    /* Assuming a filename will not be more than 50 characters */
    char *filename = (char *) malloc(50 * sizeof(char));

    sprintf(filename, "%s-%04d.ppm", prefix, frame_counter);

    fprintf(stderr, "Wrote frame #%d\n", frame_counter);

    FILE *outFile = fopen(filename, "w");

    if (outFile == NULL)
    {
        fprintf(stderr, "Pointer to outfile is NULL. Exiting.\n");
        return;
    }

    char *dimension_string = (char *) malloc(10 * sizeof(char));

    sprintf(dimension_string, "%d %d", width, height);

    fputs("P6\n", outFile);
    fputs(dimension_string, outFile);
    fputs("\n255\n", outFile);

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

    /* Cleanup */
    free(filename);
    free(dimension_string);

    fclose(outFile);
}

/* Modified version of Michael Dipperstein's PackBits algorithm variant - http://michael.dipperstein.com/rle/index.html */
void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data, int width, int height)
{
    int countChar;
    int currChar;
    int value_counter = 0;
    int min_run_length = 3;

    while (value_counter < (width * height * 3))
    {
        countChar = fgetc(rlefile);
        countChar = (char)countChar;

        if (countChar < 0)
        {
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
    /* For show how many total values (i.e individual R, G, and B values) read for this frame */
    fprintf(stderr, "Total frame values: %d\n", value_counter);
}

void separate_channel_values(unsigned char *key_frame_data,
                             unsigned char *red_frame_data,
                             unsigned char *green_frame_data,
                             unsigned char *blue_frame_data,
                             int image_pixels)
{
    // printf("separate_channel_values start\n");
    int r_index = 0;
    int g_index = 0;
    int b_index = 0;

    for (int i = 0; i < (image_pixels * 3); i++)
    {
        if (i < image_pixels)
        {
            red_frame_data[r_index] = key_frame_data[i];
            r_index++;
        }
        else if (i < (image_pixels * 2))
        {
            green_frame_data[g_index] = key_frame_data[i];
            g_index++;
        }
        else if (i < (image_pixels * 3))
        {
            blue_frame_data[b_index] = key_frame_data[i];
            b_index++;
        }
    }
    // printf("separate_channel_values end\n");
}

void decode_to_ppm(char **argv)
{
    fprintf(stderr, "Decode to ppm\n");

    FILE *rlefile = fopen(argv[1], "rb");
    if (rlefile == NULL)
    {
        fprintf(stderr, "*rlefile is a null pointer. Exiting.\n");
        return;
    }

    /* Variables for image decompression */
    int *dimensions = NULL;
    int width;
    int height;
    int image_pixels;

    /* Variables for data output */
    char *prefix = argv[2];
    unsigned char *red_frame_data = NULL;
    unsigned char *green_frame_data = NULL;
    unsigned char *blue_frame_data = NULL;
    unsigned char *key_frame_data = NULL;

    /* Determine the size of the image in the rle video file */
    dimensions = (int *) malloc(2*sizeof(int));
    get_dimensions(rlefile, dimensions);
    width = dimensions[0];
    height = dimensions[1];
    image_pixels = width * height;
    fprintf(stderr, "width: %d, height: %d, pixels:%d\n", width, height, image_pixels);

    /* Initialise the arrays to store decompressed frame data */
    red_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    green_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    blue_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));

    key_frame_data = (unsigned char *) malloc((image_pixels * 3) * sizeof(unsigned char));

    const char key_frame_delim = 'K';
    char c = '\0';
    int frame_counter = 1;

    /* Read characters from file until there is nothing else to read */
    while ((c = fgetc(rlefile)) != EOF)
    {
        /* Look for the next key frame */
        // TODO check for "E"
        if (c == key_frame_delim)
        {
            /* We've found the next key frame */
            decompress_and_store_key_frame_data(rlefile, key_frame_data, width, height);

            /* Copy separate channel values into their own arrays */
            separate_channel_values(key_frame_data, red_frame_data, green_frame_data, blue_frame_data, image_pixels);

            /* Send decompressed data to a ppm file */
            send_frame_to_ppm(width, height, red_frame_data, green_frame_data, blue_frame_data, prefix, frame_counter);

            frame_counter++;
        }
    }

    /* Cleanup */
    free(dimensions);
    free(red_frame_data);
    free(green_frame_data);
    free(blue_frame_data);
    free(key_frame_data);

    fclose(rlefile);
}

void decode_to_stdout(char **argv)
{
    fprintf(stderr, "Decode to stdout\n");

    /* Open the rle video file and test the pointer is not NULL */
    FILE *rlefile = fopen(argv[1], "rb");
    if (rlefile == NULL)
    {
        fprintf(stderr, "*rlefile is a null pointer. Exiting.\n");
        return;
    }

    int *dimensions = NULL;
    int width;
    int height;
    int image_pixels;

    unsigned char *red_frame_data = NULL;
    unsigned char *green_frame_data = NULL;
    unsigned char *blue_frame_data = NULL;
    unsigned char *key_frame_data = NULL;

    /* Determine the size of the image in the rle video file */
    dimensions = (int *) malloc(2*sizeof(int));
    get_dimensions(rlefile, dimensions);
    width = dimensions[0];
    height = dimensions[1];
    image_pixels = width * height;
    fprintf(stderr, "width: %d, height: %d, pixels:%d\n", width, height, image_pixels);

    /* Initialise the arrays to store decompressed frame data */
    red_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    green_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    blue_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));

    key_frame_data = (unsigned char *) malloc((image_pixels * 3) * sizeof(unsigned char));

    const char key_frame_delim = 'K';
    char c = '\0';

    /* Read characters from file until there is nothing else to read */
    while ((c = fgetc(rlefile)) != EOF)
    {
        /* Look for the next key frame */
        // TODO check for "E"
        if (c != 'E')
        {
            /* We've found the next key frame */
            decompress_and_store_key_frame_data(rlefile, key_frame_data, width, height);

            /* Copy separate channel values into their own arrays */
            separate_channel_values(key_frame_data, red_frame_data, green_frame_data, blue_frame_data, image_pixels);

            /* Send decompressed data to stdout */
            send_frame_to_stdout(width, height, red_frame_data, green_frame_data, blue_frame_data);

            /* Separate each frame with the integer -1 */
            int frame_separator = -1;
            printf("%d\n", frame_separator);
        }
    }

    /* Cleanup */
    free(red_frame_data);
    free(green_frame_data);
    free(blue_frame_data);
    free(key_frame_data);
    free(dimensions);

    fclose(rlefile);
}

/* UNUSED */

// void read_rle_contents() {

// }

// void adjust_scale() {

// }

// void scalefactor_is_valid() {

// }

// void apply_tween() {

// }

// void tweenfactor_is_valid() {

// }