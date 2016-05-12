#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "arg_parser.h"

/************************************************************
*               COMMAND LINE ARGUMENT VALIDATION
************************************************************/

/* Validate a command line argument. */
int validate_arg(int arg_index, char **argv)
{
    /* Variables */
    int arg1 = 0;
    int arg2 = 0;
    int arg3 = 0;
    int arg4 = 0;

    switch(arg_index) {
        /* arg1 must be a positive integer */
        case 1:
            arg1 = atoi(argv[1]);
            if (arg1 <= 0)
            {
                printf("Error: validate_arg() - arg1 is not valid.\n");
                return -1;
            }
            break;

        /* arg2 must be an integer between 0 and 100 (inclusive) */
        case 2:
            arg2 = atoi(argv[2]);
            if ((arg2 < 0) || (arg2 > 100))
            {
                printf("Error: validate_arg() - arg2 is not valid.\n");
                return -1;
            }
            break;

        /* arg3 must be an integer between 0 and 100 (inclusive) */
        case 3:
            arg3 = atoi(argv[3]);
            if ((arg3 < 0) || (arg3 > 100))
            {
                printf("Error: validate_arg() - arg3 is not valid.\n");
                return -1;
            }
            break;

        /* arg3 must be an integer between 0 and 100 (inclusive) */
        case 4:
            arg4 = atoi(argv[4]);
            if ((arg4 < 0) || (arg4 > 100))
            {
                printf("Error: validate_arg() - arg4 is not valid.\n");
                return -1;
            }
            break;

        /* Handle incorrect arg_index */
        default:
            fprintf(stderr, "Error: validate_args() - invalid arg_index: %d\n", arg_index);
            return -1;
    }

    return 0; // success!
}

int validate_args(int argc, char **argv)
{

    /* Check if correct number of arguments are passed in */
    if (check_number_of_args(argc) != 0)
    {
        return -1;
    }

    /* Validate the command line arguments */
    for (int arg_index = 1; arg_index < argc; arg_index++)
    {
        if (validate_arg(arg_index, argv) != 0)
        {
            return -1;
        }
    }

    return 0; // success!
}

int check_number_of_args(int argc)
{
    if ((argc <= 1) || (argc > 5))
    {
        /* Note: (argc - 1) is used because argv[0] is './ppmplayer' */
        fprintf(stderr, "Error: check_number_of_args() - Incorrect number of arguments: %d.\n", (argc - 1));
        return -1;
    }
    
    return 0; // success!
}

