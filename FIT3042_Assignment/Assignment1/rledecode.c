#include <stdlib.h>
#include <stdio.h>
#include "arg_parser.h"
#include "rledecode.h"

int rledecode(int argc, char **argv)
{
    printf("--- Starting rledecode ---\n");

    if (validate_args(argc, argv) != 0) {
        printf("Args are not valid.\n");
        exit_program();
    }
    return 0;
}

int validate_args(int argc, char **argv)
{
    int result = check_number_of_args(argc);
    printf("check_number_of_args: %d\n", result);
    return result;
}

int check_number_of_args(int argc)
{
    int test = correct_number_of_args(argc);
    if (test)
    {
        return 0;
    }
    else {
        printf("Incorrect number of arguments: %d.\n", (argc - 1)); // (argc - 1) because argv[0] is './rledecode'
        return -1;
    }
}

int correct_number_of_args(int argc)
{
    // Remember: argv[0] is './rledecode'
    return (argc <= 5 && argc >= 3); // returns 1 if success, 0 otherwise
}

void exit_program()
{
    printf("Exiting Cleanly.\n");
    exit(EXIT_SUCCESS);
}


/* UNUSED */

void read_rle_contents() {

}

void adjust_scale() {

}

void scalefactor_is_valid() {

}

void apply_tween() {

}

void tweenfactor_is_valid() {

}