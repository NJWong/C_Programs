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
    Output: 0 on success, -1 on failure
*/

int parse_arg(int arg_index, char *arg) {

    switch(arg_index) {
        case 1:
            return handle_arg1(arg);
        case 2:
            return handle_arg2(arg);
        case 3:
            return handle_arg3(arg);
        case 4:
            return handle_arg4(arg);
        default:
            printf("Error: parse_argument - incorrect arg_index");
            return -1;
    }
}



int parse_arguments(int argc, char **argv)
{

    int arg1_flag, arg2_flag, arg3_flag, arg4_flag;

    /* Check if 'rleplay' file exists */
    arg1_flag = handle_arg1(argv[1]);
    if (arg1_flag == -1)
    {
        printf("Argument 1 Error.\n");
        return -1;
    }

    /* Determine which output method to use */
    arg2_flag = handle_arg2(argv[2]);
    if (arg2_flag == 1)
    {
        output_to_stdout();
    }
    else if (arg2_flag == 2)
    {
        output_to_files();
    }
    else if (arg2_flag == -1)
    {
        printf("Argument 2 Error.\n");
        return -1;
    }

    // TODO remove
    arg3_flag = 0;
    arg4_flag = 0;

    /* Handle optional arguments */
    if (argc == 4)
    {        /* at least one optional arg */
        handle_arg3(argv[3]);

        if (argc == 5)
        {    /* both optional args */
            handle_arg4(argv[4]);
        }
    }
    return 0;
}

int decode_rlefile(FILE *infile, FILE *outfile)
{
    return 0;
}

void output_to_stdout()
{
    printf("Output: to stdout...\n");
}

void output_to_files()
{
    printf("Output: to files...\n");
}

/* Handler and methods for argv[1] */

/*
    Determines if the filename from the first command line argument exists.
*/
int handle_arg1(char *filename)
{
    // Setup
    // TODO check this - it is dangerous to assume that rleplay files will be in this folder
    char *path = create_path("rlefiles/", filename);

    if (strcmp(path, "NullError") == 0)
    {
        printf("Error: Either folder or filename is NULL. Exiting cleanly.\n");
        return -1;
    }

    printf("Looking for %s...\n", path);

    if (rleplay_file_exists(path))
    {
        printf("%s exists. Proceeding...\n", path);
    }
    else
    {
        printf("%s does not exist.\n", path);
        return -1;
    }
    // Cleanup
    free(path);

    return 0;
}

char * create_path(char *folder, char *filename)
{
    if (folder == NULL || filename == NULL)
    {
        return "NullError";
    }

    // TODO maybe do the malloc and NULL check in a reusable method
    char *path = (char *) malloc(1 + strlen(folder) + strlen(filename));

    // Another NULL test can't hurt
    if (path == NULL)
    {
        printf("Error: Path to %s is NULL. Exiting cleanly.\n", filename);
        exit(EXIT_SUCCESS);
    }
    else
    {
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
int rleplay_file_exists(char *filename)
{
    struct stat buffer;
    if (filename != NULL)
    {
        return (stat (filename, &buffer) >= 0); // stat returns a negative value on failure
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