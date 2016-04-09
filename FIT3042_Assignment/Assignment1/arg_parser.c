#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "arg_parser.h"

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

/*
    Check for the existance of a 'rleplay' file inside the 'rlefiles' folder.

    Assumptions:
        * The size of a 'rleplay' is < 2GB since stat() has documented problems in this case

    Input: Name of the 'rleplay' file
    Output: 1 if the file exists, 0 otherwise
*/
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

/* BELOW IS OUT OF DATE CODE */


// int parse_arguments(int argc, char **argv)
// {

//     int arg1_flag, arg2_flag, arg3_flag, arg4_flag;

//     /* Check if 'rleplay' file exists */
//     arg1_flag = handle_arg1(argv[1]);
//     if (arg1_flag == -1)
//     {
//         printf("Argument 1 Error.\n");
//         return -1;
//     }

//     /* Determine which output method to use */
//     arg2_flag = handle_arg2(argv[2]);
//     if (arg2_flag == 1)
//     {
//         output_to_stdout();
//     }
//     else if (arg2_flag == 2)
//     {
//         output_to_files();
//     }
//     else if (arg2_flag == -1)
//     {
//         printf("Argument 2 Error.\n");
//         return -1;
//     }

//     // TODO remove
//     arg3_flag = 0;
//     arg4_flag = 0;

//     /* Handle optional arguments */
//     if (argc == 4)
//     {        /* at least one optional arg */
//         handle_arg3(argv[3]);

//         if (argc == 5)
//         {    /* both optional args */
//             handle_arg4(argv[4]);
//         }
//     }
//     return 0;
// }

// int decode_rlefile(FILE *infile, FILE *outfile)
// {
//     return 0;
// }

// void output_to_stdout()
// {
//     printf("Output: to stdout...\n");
// }

// void output_to_files()
// {
//     printf("Output: to files...\n");
// }

// char * create_path(char *folder, char *filename)
// {
//     // TODO move the malloc operation and NULL check to a reusable method
//     if (folder == NULL || filename == NULL)
//     {
//         return "NullError";
//     }

//     // Allocate memory for char *path
//     char *path = (char *) malloc(1 + strlen(folder) + strlen(filename));

//     strcpy(path, folder);
//     strcpy(path, filename);

//     return path;

//     // Double check we are not trying to write null to path
//     // if (path == NULL)
//     // {
//     //     printf("Error: Path to %s is NULL. Exiting cleanly.\n", filename);
//     //     exit(EXIT_SUCCESS);
//     // }
//     // else
//     // {
//     //     strcpy(path, folder);
//     //     strcat(path, filename);
//     //     return path;
//     // }
// }