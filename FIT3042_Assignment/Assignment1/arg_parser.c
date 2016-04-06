#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <arg_parser.h>

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
        handle_arg3(argv[3]);

        if (argc == 5) {    // both optional args
            handle_arg4(argv[4]);
        }
    }
}

/* Handler and methods for argv[3] */

/*
    Determines if the filename from the first command line argument exists.
*/
void handle_arg1(char *filename) {
    // Setup
    char *path = create_path("rlefiles/", filename);

    if (strcmp(path, "NullError") == 0) {
        printf("Error: Either folder or filename is NULL. Exiting cleanly.\n");
        exit(EXIT_SUCCESS);
    }

    printf("Looking for %s...\n", path);

    if (rleplay_file_exists(path)) {
        printf("%s exists. Proceeding...\n", path);
    } else {
        printf("Error: %s does not exist. Exiting cleanly.\n", path);
        exit(EXIT_SUCCESS);
    }
    // Cleanup
    free(path);
}

char * create_path(char *folder, char *filename) {
    if (folder == NULL || filename == NULL) {
        return "NullError";
    }

    // TODO maybe do the malloc and NULL check in a reusable method
    char *path = (char *) malloc(1 + strlen(folder) + strlen(filename));

    // Another NULL test can't hurt
    if (path == NULL) {
        printf("Error: Path to %s is NULL. Exiting cleanly.\n", filename);
        exit(EXIT_SUCCESS);
    } else {
        strcpy(path, folder);
        strcat(path, filename);
        return path;
    }
}

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

/* Handler and methods for argv[2] */
void handle_arg2(char *arg2) {
    if (strcmp(arg2, "-") == 0) {
        output_to_stdout();
    } else if (is_valid_prefix(arg2)) {
        output_to_files();
    } else {
        printf("invalid value for arg2\n");
    }
}

int is_valid_prefix(char *prefix) {
    return 1;
}

void output_to_files() {
    printf("Output: to files\n");
}

void output_to_stdout() {
    printf("output to stdout\n");
}

/* Handler and methods for argv[3] */
void handle_arg3(char *arg3) {

}

/* Handler and methods for argv[4] */
void handle_arg4(char *arg4) {

}