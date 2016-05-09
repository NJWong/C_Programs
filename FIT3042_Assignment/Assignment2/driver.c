#include "ppmplayer.h"

/************************************************************
* Desc   : This is the entry point to the program. It takes
*		   in the command line arguments and passes it to
*		   the ppmplayer function to process.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for completion (not necessarily program success)
************************************************************/
int main(int argc, char **argv)
{
    ppmplayer(argc, argv);
    return 0;
}