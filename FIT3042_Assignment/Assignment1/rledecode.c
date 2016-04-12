#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rledecode.h"
#include "arg_validator.h"
#include "decoder.h"

/************************************************************
* Desc   : Primary function that determines whether the
*          command line arguments are valid and then initiates
*          the rle file decode process.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure. The stderr output
*          stream is used for showing helpful messages to
*          the user.
************************************************************/
int rledecode(int argc, char **argv)
{
    fprintf(stderr, "\n--- Starting rledecode ---\n");

    /* Check that the command line arguments are valid */
    if (validate_args(argc, argv) != 0)
    {
        /* If invalid, then stop here and return to main */
        fprintf(stderr, "Args are not valid.\n");
        return -1;
    }

    /* Decode the .rle file and put to output defined in arg2 */
    decode_rlefile(argv);

    return 0;
}

/* UNUSED */

// void adjust_scale() {

// }

// void scalefactor_is_valid() {

// }

// void apply_tween() {

// }

// void tweenfactor_is_valid() {

// }