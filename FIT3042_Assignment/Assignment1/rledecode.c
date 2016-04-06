#include <stdlib.h>
#include <stdio.h>
// #include "arg_parser.c"
#include <arg_parser.h>

int validate_args(int argc, char **argv);
int correct_number_of_args(int argc);
void exit_program();

/* Entry point */
int main(int argc, char **argv)
{
    printf("--- START rledecode ---");

    validate_args(argc, argv);

    printf("--- END rledecode ---");
    return 0;
}

int validate_args(int argc, char **argv)
{
    int pa_flag = 0;

    if (correct_number_of_args(argc))
    {
        pa_flag = parse_arguments(argc, argv);
        if (pa_flag == -1)
        {
            exit_program();
        }
    }
    else
    {
        printf("Incorrect number of arguments: %d.\n", (argc - 1)); // (argc - 1) because argv[0] is './rledecode'
        exit_program();
    }

    return 0;
}

/*
    Check there are the correct number of arguments.
    There are two required arguments, and two optional arguments for rledecode.
    
    Input: the length of argv - the argument list
    Output: 1 if there are the correct number of arguments, 0 otherwise
*/
int correct_number_of_args(argc)
{
    // Remember: argv[0] is './rledecode'
    return (argc <= 5 && argc >= 3);
}

void exit_program()
{
    printf("Exiting Cleanly.\n");
    exit(EXIT_SUCCESS);
}

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