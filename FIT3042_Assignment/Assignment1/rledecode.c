#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rledecode.h"

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
    if ((strcmp(argv[2], "-") == 0))
        decode_to_stdout(argc, argv);
    else
        decode_to_ppm_files(argc, argv);

    return 0;
}

void decode_to_stdout(int argc, char **argv)
{
    fprintf(stderr, "decode_to_stdout\n");
}

void decode_to_ppm_files(int argc, char **argv)
{
    fprintf(stderr, "decode_to_ppm_files\n");
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
            printf("Error: parse_argument - incorrect arg_index");
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
    printf("Looking for %s...\n", filepath);

    if (rleplay_file_exists(filepath))
    {
        printf("%s exists. Proceeding...\n", filepath);
    }
    else
    {
        printf("%s does not exist.\n", filepath);
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
        printf("Invalid value for arg2\n");
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
    printf("handle_arg3");
    return 0;
}

/* Handler and methods for argv[4] */
int handle_arg4(char *arg4)
{
    printf("handle_arg4");
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