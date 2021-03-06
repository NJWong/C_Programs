#include <stdio.h>
#include "ppmplayer.h"
#include "arg_parser.h"
#include "video_player.h"

/************************************************************
* Desc   : Primary function that determines whether the
*          command line arguments are valid and then initiates
*          the video_player.
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
    fprintf(stderr, "--- Starting ppmplayer ---\n\n");

    /* Check that the command line arguments are valid */
    if (validate_args(argc, argv) != 0)
    {
        /* If invalid, then stop here and return to main */
        printf("Error: Args are not valid.\n");
        return -1;
    }

    /* Initiate the video player */
    if(video_player_init(argc, argv) != 0)
    {
        printf("Warning: Video did not finish playing.\n");
        return -1;
    }

    printf("\n--- Closing ppmplayer ---\n");
    return 0;
}