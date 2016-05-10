#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ppmplayer.h"
#include "arg_parser.h"
#include "video_player.h"

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

    /* Initiate the video player */
    if(play_video(argv) != 0)
    {
        printf("Video could not be played.\n");
    }

    return 0;
}