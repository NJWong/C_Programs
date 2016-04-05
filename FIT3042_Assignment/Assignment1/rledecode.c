#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/*
    Check for the existance of a 'rleplay' file inside the 'rlefiles' folder.

    Assumptions:
        * The size of a 'rleplay' is < 2GB since stat() has documented problems in this case

    Input: Name of the 'rleplay' file
    Output: 1 if the file exists, 0 otherwise
*/
int rleplay_file_exists(char *filename) {
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

/*
    Determines if the filename from the first command line argument exists.
*/
void handle_arg1(char *filename) {

    // Setup
    char *rleplay_folder = "rlefiles/";

    char *path = (char *) malloc(1 + strlen(rleplay_folder) + strlen(filename));
    strcpy(path, rleplay_folder);
    strcat(path, filename);

    printf("Looking for %s...\n", path);

    if (rleplay_file_exists(path)) {
        printf("%s exists. Proceeding...\n", path);
    } else {
        printf("%s does not exist. Exiting cleanly.\n", path);
        exit(EXIT_SUCCESS);
    }

    // Cleanup
    free(path);
}

void output_to_files() {
    printf("output to files\n");
}

void output_to_stdout() {
    printf("output to stdout\n");
}

int is_valid_prefix(char *prefix) {
    return 1;
}

void handle_arg2(char *arg2) {
    if (!strcmp(arg2, "-")) {
        output_to_stdout();
    } else if (is_valid_prefix(arg2)) {
        output_to_files();
    } else {
        printf("invalid value for arg2\n");
    }
}

void handle_arg3() {

}

void handle_arg4() {

}

/*
	Parses the command line argument list and executes appropriate functions.

    Assumptions:
        * argv[1] will always be the rlepaly filename
        * argv[2] will always either be '-' or '<prefix>'
            - if one is used, the other cannot be used at the same time      
        * argv[3] can either be '--scale <scalefactor>' or '-tween <tweenfactor>'
        * argv[4] can either be '--scale <scalefactor>' or '-tween <tweenfactor>'
            - if argv[3] is one of these, then argv[4] must be the other

	Input: argc - the length of the command line argument list
           argv - The commend line argument list
	Output: Nothing. Output is handled by appropriate functions
*/
void parse_arguments(int argc, char **argv) {

    // Check if 'rleplay' file exists
    handle_arg1(argv[1]);

    // Determine which output method to use
    handle_arg2(argv[2]);

    // Handle optional arguments
    if (argc == 4) {        // at least one optional arg
        handle_arg3();

        if (argc == 5) {    // both optional args
            handle_arg4();
        }
    }
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

/*
	Program entry point

    Assumptions:
        * 
*/
int main(int argc, char **argv) {

    if (correct_number_of_args(argc))
        parse_arguments(argc, argv);
    else
        printf("Incorrect number of arguments: %d.\n", argc);

    return 0;
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