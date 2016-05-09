#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "arg_parser.h"

/************************************************************
*               COMMAND LINE ARGUMENT VALIDATION
************************************************************/

/************************************************************
* Desc   : Function that checks whether the command line
*          arguments are valid or not.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int validate_args(int argc, char **argv)
{
    int correct_number_of_args = check_number_of_args(argc);
    if (correct_number_of_args == -1)
    {
        return -1;
    }

    return 0;
}

/************************************************************
* Desc   : Function that checks if there is the correct
*          number of arguments passed through the command line
*          Minimum is 1 - the delay (in ms) between frames
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int check_number_of_args(int argc)
{
    if (argc == 2)
    {
        return 0;
    }
    else {
        /* Note: (argc - 1) is used because argv[0] is './rledecode' */
        fprintf(stderr, "Incorrect number of arguments: %d.\n", (argc - 1));
        return -1;
    }
}

/************************************************************
* Desc   : Function that deligates which argument to handle
*          to the appropriate handler function.
*
* Params : arg_index - the 
*          argc - the number of given command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int parse_arg(int arg_index, char **argv)
{
    /* Flag that determines success or failure */
    int flag = -1;

    // switch(arg_index) {
    //     case 1:
    //         flag = handle_arg1(argv[1]);
    //         break;
    //     case 2:
    //         flag = handle_arg2(argv[2]);
    //         break;
    //     case 3:
    //         flag = handle_arg3(argv[3], argv[4]);
    //         break;
    //     default:
    //         fprintf(stderr, "Error: parse_argument - incorrect arg_index");
    //         break;
    // }

    return flag;
}