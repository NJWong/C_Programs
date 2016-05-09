#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ppmplayer.h"
#include "arg_parser.h"

/************************************************************
* Desc   : Primary function that determines whether the
*          command line arguments are valid and then initiates
*          the ppmplayer process.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure. The stderr output
*          stream is used for showing helpful messages to
*          the user.
************************************************************/
int ppmplayer(int argc, char **argv)
{
    fprintf(stderr, "\n--- Starting ppmplayer ---\n");

    /* Check that the command line arguments are valid */
    if (validate_args(argc, argv) != 0)
    {
        /* If invalid, then stop here and return to main */
        fprintf(stderr, "Args are not valid.\n");
        return -1;
    }

    /* Determine if a modifier ('--scale' and/or '--tween') is passed in */
    // int num_of_mods = 0;

    /* If there are 5 arguments, then there is a modifier */
    // if (argc == 5)
    // {
    //     num_of_mods = 1;
    // }

    /* Decode the .rle file and put to output defined in arg2 and with number of modifiers */
    // decode_rlefile(argv, num_of_mods);

    return 0;
}