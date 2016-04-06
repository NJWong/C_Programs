#include <stdlib.h>
#include <stdio.h>
// #include "arg_parser.c"
#include <arg_parser.h>

int correct_number_of_args(int argc);

/*
    Program entry point
*/
int main(int argc, char **argv) {

    if (correct_number_of_args(argc))
        parse_arguments(argc, argv);
    else
        printf("Incorrect number of arguments: %d.\n", (argc - 1)); // (argc - 1) because argv[0] is './rledecode'

    return 0;
}

/*
    Check there are the correct number of arguments.
    There are two required arguments, and two optional arguments for rledecode.
    
    Input: the length of argv - the argument list
    Output: 1 if there are the correct number of arguments, 0 otherwise
*/
int correct_number_of_args(argc) {
    // Remember: argv[0] is './rledecode'
    return (argc <= 5 && argc >= 3);
}

void read_rle_contents() {

}

void convert_to_ppm() {

}

void adjust_scale() {

}

void scalefactor_is_valid() {

}

void apply_tween() {

}

void tweenfactor_is_valid() {

}