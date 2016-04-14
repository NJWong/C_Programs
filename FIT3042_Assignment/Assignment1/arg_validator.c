#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "arg_validator.h"

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

    int arg1_is_valid = parse_arg(1, argv);
    int arg2_is_valid = parse_arg(2, argv);
    int arg3_is_valid = 0; // default value
    // int arg4_is_valid = 0; // default value

    if (argc == 5)
    {
        arg3_is_valid = parse_arg(3, argv);
    }
    
    if (argc == 7)
    {
        arg4_is_valid = parse_arg(4, argv);
    }

    /* Check argv[3] and argv[4] are not both scale or tween */
    // TODO

    /* Note: the '!' is there so that we return 0 when all args are valid */
    return !((arg1_is_valid == 0) && ((arg2_is_valid == 1) || (arg2_is_valid == 2)) && (arg3_is_valid == 0));
}

/************************************************************
* Desc   : Function that checks if there is the correct
*          number of arguments passed through the command line
*          Minimum is 3:
*          ./rledecode <*.rle> (<prefix> || "-")
*          Maximum is 7:
*          ./rledecode <*.rle> (<prefix> || "-") --scale <sf> --tween <tf>
*          4 or 6 is strictly disallowed
           i.e. argc can only be 3, 5, and 7
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int check_number_of_args(int argc)
{
    if (argc == 3 || argc == 5 || argc == 7)
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

    switch(arg_index) {
        case 1:
            flag = handle_arg1(argv[1]);
            break;
        case 2:
            flag = handle_arg2(argv[2]);
            break;
        case 3:
            flag = handle_arg3(argv[3], argv[4]);
            break;
        // case 4:
        //     flag = handle_arg4(argv[5], argv[6]);
        //     break;
        default:
            fprintf(stderr, "Error: parse_argument - incorrect arg_index");
            break;
    }

    return flag;
}

/************************************************************
* Desc   : Function that determines if argv[1] is valid or
*          not by checking if that rle file exists. Output
*          messages are put to the stderr stream for the
*          user to see. This keeps the stdout stream clean.
*
* Params : filepath - relative path to the rle file
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int handle_arg1(char *filepath)
{
    /* Handle empty string and NULL pointer */
    if (strcmp(filepath, "") == 0 || filepath == NULL) {
        return -1;
    }

    fprintf(stderr, "Looking for %s...\n", filepath);

    /* Determine if the rle file exists or not */
    if (rleplay_file_exists(filepath))
    {
        fprintf(stderr, "%s exists. Proceeding...\n", filepath);
    }
    else
    {
        fprintf(stderr, "%s does not exist.\n", filepath);
        return -1;
    }

    return 0;
}

/************************************************************
* Desc   : Function that checks whether or not a file exists
*          using the 'stat' system call. If 'stat' is called
*          on a file that does not exist, then it failes.
*          Thus, we can easily check a file's existance.
*
* Params : filepath - relative path to the rle file
*           
* Return : positive int for success, negative int for failure.
************************************************************/
int rleplay_file_exists(char *filepath)
{
    struct stat buffer;

    /* First check if NULL */
    if (filepath != NULL)
    {
        /* Note: stat returns a negative value on failure */
        return (stat (filepath, &buffer) >= 0);
    }

    /* If NULL, then we return act as if stat failed */
    return -1;
}

/************************************************************
* Desc   : Function that determines if argv[2] is valid or
*          not by checking if it is a dash "-" or a valid
*          prefix string.
*
* Params : arg2 - the second command line argument we get
*           
* Return : 1 for "-", 2 for valid prefix, -1 for invalid prefix.
************************************************************/
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
        fprintf(stderr, "Invalid value for arg2\n");
        return -1;
    }
}

/************************************************************
* Desc   : Function that determines if the prefix is valid or
*          not. At this stage, the only invalid prefix is the
*          empty string.
*
* Params : prefix - the prefix string we want to validate
*           
* Return : 1 for success, 0 for failure.
************************************************************/
int is_valid_prefix(char *prefix)
{
    return (strlen(prefix) > 0);
}

/************************************************************
* Desc   : Function that determines if argv[3] is valid or
*          not by checking if it matches one of the two
*          optional arguments.
*
* Params : arg3 - the third command line argument we get
*          factor - the tween or scale factor used for linear
*                   interpolation - is a positive integer
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int handle_arg3(char *arg3, char *factor)
{
    /* If arg3 and the  is valid then return 0 */
    if ((strcmp(arg3, "--scale") == 0 || strcmp(arg3, "--tween") == 0) && (atoi(factor) > 0))
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "arg3 and associated factor are not valid.\n");
        return -1;
    }
}

/************************************************************
* Desc   : Function that determines if argv[4] is valid or
*          not by checking if it matches one of the two
*          optional arguments.
*
* Params : arg4 - the third command line argument we get
*           
* Return : 0 for success, 1 for failure.
************************************************************/
// int handle_arg4(char *arg4, char *factor)
// {
//     fprintf(stderr, "handle_arg4");
//     return 0;
// }