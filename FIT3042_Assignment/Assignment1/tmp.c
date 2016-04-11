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