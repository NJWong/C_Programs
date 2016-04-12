#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rledecode.h"

/************************************************************
* Desc   : Primary function that determines whether the
*          command line arguments are valid and then initiates
*          the rle file decode process.
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
    decode_rlefile(argv);

    return 0;
}
/************************************************************
*               COMMAND LINE ARGUMENT VALIDATION
************************************************************/

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

    int arg1_is_valid = parse_arg(1, argv);
    int arg2_is_valid = parse_arg(2, argv);
    int arg3_is_valid;
    int arg4_is_valid;

    if (argc == 5)
    {
        arg3_is_valid = parse_arg(3, argv);
    }
    
    if (argc == 7)
    {
        arg4_is_valid = parse_arg(4, argv);
    }

    /* Check argv[3] and argv[4] are not both scale or tween */
    // TODO

    /* Note: the '!' is there so that we return 0 when all args are valid */
    return !((arg1_is_valid == 0) && ((arg2_is_valid == 1) || (arg2_is_valid == 2)) && (arg3_is_valid == 0));
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
int parse_arg(int arg_index, char **argv)
{
    /* Flag that determines success or failure */
    int flag = -1;

    switch(arg_index) {
        case 1:
            flag = handle_arg1(argv[1]);
            break;
        case 2:
            flag = handle_arg2(argv[2]);
            break;
        case 3:
            flag = handle_arg3(argv[3], argv[4]);
            break;
        case 4:
            flag = handle_arg4(argv[5], argv[6]);
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
* Desc   : Function that checks whether or not a file exists
*          using the 'stat' system call. If 'stat' is called
*          on a file that does not exist, then it failes.
*          Thus, we can easily check a file's existance.
*
* Params : filepath - relative path to the rle file
*           
* Return : positive int for success, negative int for failure.
************************************************************/
int rleplay_file_exists(char *filepath)
{
    struct stat buffer;

    /* First check if NULL */
    if (filepath != NULL)
    {
        /* Note: stat returns a negative value on failure */
        return (stat (filepath, &buffer) >= 0);
    }

    /* If NULL, then we return act as if stat failed */
    return -1;
}

/************************************************************
* Desc   : Function that determines if argv[2] is valid or
*          not by checking if it is a dash "-" or a valid
*          prefix string.
*
* Params : arg2 - the second command line argument we get
*           
* Return : 1 for "-", 2 for valid prefix, -1 for invalid prefix.
************************************************************/
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
}

/************************************************************
* Desc   : Function that determines if the prefix is valid or
*          not. At this stage, the only invalid prefix is the
*          empty string.
*
* Params : prefix - the prefix string we want to validate
*           
* Return : 1 for success, 0 for failure.
************************************************************/
int is_valid_prefix(char *prefix)
{
    return (strlen(prefix) > 0);
}

/************************************************************
* Desc   : Function that determines if argv[3] is valid or
*          not by checking if it matches one of the two
*          optional arguments.
*
* Params : arg3 - the third command line argument we get
*          factor - the tween or scale factor used for linear
*                   interpolation - is a positive integer
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int handle_arg3(char *arg3, char *factor)
{
    /* If arg3 and the  is valid then return 0 */
    if ((strcmp(arg3, "--scale") == 0) && (atoi(factor) > 0))
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "arg3 and associated factor are not valid.\n");
        return -1;
    }
}

/************************************************************
* Desc   : Function that determines if argv[4] is valid or
*          not by checking if it matches one of the two
*          optional arguments.
*
* Params : arg4 - the third command line argument we get
*           
* Return : 0 for success, 1 for failure.
************************************************************/
int handle_arg4(char *arg4, char *factor)
{
    fprintf(stderr, "handle_arg4");
    return 0;
}

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
void decode_rlefile(char **argv)
{
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
            decompress_and_store_key_frame_data(rlefile, key_frame_data, width, height);

            /* Copy separate channel values into their own arrays */
            separate_channel_values(key_frame_data, red_frame_data, green_frame_data, blue_frame_data, image_pixels);

            /* Output using the method specified in argv[2] */
            if (to_stdout)
            {
                /* Send decompressed data to stdout */
                send_frame_to_stdout(width, height, red_frame_data, green_frame_data, blue_frame_data);

                /* Separate each frame with the integer -1 */
                int frame_separator = -1;
                /* Note: printf() outputs to the stdout stream */
                printf("%d\n", frame_separator);   
            }
            else
            {
                /* Send decompressed data to a ppm file */
                send_frame_to_ppm(width, height, red_frame_data, green_frame_data, blue_frame_data, prefix, frame_counter);    
                frame_counter++;
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
void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data, int width, int height)
{
    int countChar;
    int currChar;
    int value_counter = 0;
    int min_run_length = 3;

    /* Decode exactly the number of values required for a single frame */
    /* Note: We know the width and height, and that the R, G, and B values are stored
       separately, so the number of values required is 'width * height * 3'. */
    while (value_counter < (width * height * 3))
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

/* UNUSED */

// void adjust_scale() {

// }

// void scalefactor_is_valid() {

// }

// void apply_tween() {

// }

// void tweenfactor_is_valid() {

// }