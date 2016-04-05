#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
	Parses the command line argument list and executes appropriate functions.

	Input: The commend line argument list
	Output: Nothing. Output is handled by appropriate functions
*/
void parse_arguments(int argc, char **argv) {

    for (int index = 0; index < argc; index++) {
        printf("argument %d : %s\n", index, argv[index]);
    }
}

int has_correct_number_of_args(argc) {
    return (argc <= 5 && argc >= 3); // argv[0] is './rledecode'
}

/*
	Program entry point
*/
int main(int argc, char **argv) {

    if (has_correct_number_of_args(argc))
        parse_arguments(argc, argv);
    else
        printf("Incorrect number of arguments: %d.\n", argc);

    return 0;
}

/*
	Check for the existance of a file.

	Input: Name of the 'rleplay' file
	Output: 1 if the file exists, 0 if it does not
*/
int rleplay_file_exists() {
	int exists;

    // dummy code
    exists = 1;

	return exists;
}

void convert_to_ppm() {

}

void output_to_files() {

}

void output_to_stdout() {

}

void adjust_scale() {

}

void scalefactor_is_valid() {

}

void apply_tween() {

}

void tweenfactor_is_valid() {

}