#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rledecode.h"

#define MIN_RUN     3                   /* minimum run length to encode */
#define MAX_RUN     (128 + MIN_RUN - 1) /* maximum run length to encode */
#define MAX_COPY    128                 /* maximum characters to copy */

/* maximum that can be read before copy block is written */
#define MAX_READ    (MAX_COPY + MIN_RUN - 1)

int rledecode(int argc, char **argv)
{
    fprintf(stderr, "\n--- Starting rledecode ---\n");

    /* Check that the commandline arguments are valid */
    if (validate_args(argc, argv) != 1)
    {
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

// TODO possible split this into get_width and get_height
int * get_dimensions(FILE *rlefile)
{
    char *width_string = (char*) malloc(4*sizeof(char));
    char *height_string = (char*) malloc(4*sizeof(char));

    int line_count = 0;

    while (line_count < 3)
    {
        char line[10]; //TODO malloc instead
        fgets(line, sizeof line, rlefile);
        if (line_count == 2)
        {
            // FIX ME because the dimensions doesn't always have 4 characters
            strncpy(width_string, line, 4); 
            strncpy(height_string, &line[5], 4);
        }
        line_count++;
    }

    int *dimensions = (int *) malloc(2*sizeof(int));

    dimensions[0] = atoi(width_string);
    dimensions[1] = atoi(height_string);

    /* Cleanup */
    free(width_string);
    free(height_string);

    return dimensions;
}

void send_to_stdout(int width, int height, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data)
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

void send_to_ppm(int width, int height, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data, FILE *outFile)
{
    fputs("P6\n1200 1600\n255\n", outFile);

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
}

void packbits_decode(FILE *rlefile, int width, int height)
{
    int image_pixels = width * height;

    /* Initialise the arrays for each color channel */
    unsigned char *red_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    unsigned char *green_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));
    unsigned char *blue_frame_data = (unsigned char *) malloc(image_pixels * sizeof(unsigned char));

    unsigned char *key_frame_data = (unsigned char *) malloc((image_pixels * 3) * sizeof(unsigned char));

    const char key_frame_delim = 'K';
    char c = '\0';

    while ((c = fgetc(rlefile)) != EOF)
    {
        /* Look for the next key frame */
        if (c == key_frame_delim)
        {
            /* We've found the next key frame */
            int countChar;
            int currChar;
            int value_counter = 0;

            while ((countChar = fgetc(rlefile)) != EOF)
            {
                countChar = (char)countChar;

                if (countChar < 0)
                {
                    countChar = (MIN_RUN - 1) - countChar;

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
                else {
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

            /* Copy separate channel values into their own arrays */
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
            send_to_stdout(width, height, red_frame_data, green_frame_data, blue_frame_data);
        }

    }

    /* Cleanup */
    free(red_frame_data);
    free(green_frame_data);
    free(blue_frame_data);
    free(key_frame_data);
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

    /* Determine the size of the image in the rle video file */
    int *dimensions = get_dimensions(rlefile);
    int width = dimensions[0];
    int height = dimensions[1];
    printf("width: %d, height: %d\n", width, height);

    /* Use packbits decompression algorithm and store result in key_frame_data array */
    packbits_decode(rlefile, width, height);

    /* Cleanup */
    fclose(rlefile);
}


void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data)
{
    int countChar;
    int currChar;
    int value_counter = 0;

    while ((countChar = fgetc(rlefile)) != EOF)
    {
        countChar = (char)countChar;

        if (countChar < 0)
        {
            countChar = (MIN_RUN - 1) - countChar;

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
        else {
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
}

void separate_channel_values(unsigned char *key_frame_data, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data, int image_pixels)
{
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
}

void decode_to_ppm(char **argv)
{
    fprintf(stderr, "Decode to ppm\n");

    FILE *rlefile = fopen(argv[1], "rb");
    FILE *outFile = NULL;

    int *dimensions = NULL;
    int width;
    int height;
    int image_pixels;

    unsigned char *red_frame_data = NULL;
    unsigned char *green_frame_data = NULL;
    unsigned char *blue_frame_data = NULL;
    unsigned char *key_frame_data = NULL;

    outFile = fopen("testout.ppm", "w");

    if (rlefile == NULL || outFile == NULL)
    {
        fprintf(stderr, "*rlefile or *outFile is a null pointer. Exiting.\n");
        return;
    }

    /* Determine the size of the image in the rle video file */
    dimensions = get_dimensions(rlefile);
    width = dimensions[0];
    height = dimensions[1];
    image_pixels = width * height;
    printf("width: %d, height: %d, pixels:%d\n", width, height, image_pixels);

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
        if (c == key_frame_delim)
        {
            /* We've found the next key frame */
            decompress_and_store_key_frame_data(rlefile, key_frame_data);

            /* Copy separate channel values into their own arrays */
            separate_channel_values(key_frame_data, red_frame_data, green_frame_data, blue_frame_data, image_pixels);

            /* Send decompressed data to a ppm file */
            send_to_ppm(width, height, red_frame_data, green_frame_data, blue_frame_data, outFile);
        }
    }

    /* Cleanup */
    free(red_frame_data);
    free(green_frame_data);
    free(blue_frame_data);
    free(key_frame_data);

    fclose(outFile);
    fclose(rlefile);
}

int validate_args(int argc, char **argv)
{
    int correct_number_of_args = check_number_of_args(argc);
    int arg1_is_valid = parse_arg(1, argv[1]);
    int arg2_is_valid = parse_arg(2, argv[2]);

    return ( (correct_number_of_args == 0) &&
             (arg1_is_valid == 0) &&
             ((arg2_is_valid == 1) || arg2_is_valid == 2)
           );
}

int check_number_of_args(int argc)
{
    if (argc <= 5 && argc >= 3)
    {
        return 0;
    }
    else {
        fprintf(stderr, "Incorrect number of arguments: %d.\n", (argc - 1)); // (argc - 1) because argv[0] is './rledecode'
        return -1;
    }
}

int parse_arg(int arg_index, char *arg) {

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

/* Handler and methods for argv[1] */
int handle_arg1(char *filepath)
{
    if (strcmp(filepath, "") == 0 || filepath == NULL) {
        return -1;
    }

    // Not assuming rle files are within the 'rlefiles' directory
    fprintf(stderr, "Looking for %s...\n", filepath);

    if (rleplay_file_exists(filepath))
    {
        fprintf(stderr, "%s exists. Proceeding...\n", filepath);
    }
    else
    {
        fprintf(stderr, "%s does not exist.\n", filepath);
        return -1; // it seems 'return -1' gives a SegFault
    }    

    return 0;
}

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