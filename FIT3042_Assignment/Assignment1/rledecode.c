#include <stdlib.h>
#include <stdio.h>
// #include "arg_parser.h"
// #include "arg_parser.c"
#include "rledecode.h"

int rledecode(int argc, char **argv)
{
    fprintf(stderr, "--- Starting rledecode ---\n");

    if (validate_args(argc, argv) != 0) {
        fprintf(stderr, "Args are not valid.\n");
        return -1;
    }
    return 0;
}

int validate_args(int argc, char **argv)
{
    int correct_number_of_args = check_number_of_args(argc);
    // int can_parse_args = parse_arg(1, argv[1]);

    return correct_number_of_args;
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