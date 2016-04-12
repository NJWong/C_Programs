#include "rledecode.h"

/************************************************************
* Desc   : This is the entry point to the program. It takes
*		   in the command line arguments and passes it to
*		   the rledecode function to process.
*
*		   This is done because my test suite also has a main
*		   function, and if I imported 'rledecode.c' with a
*		   main function still there, gcc would get confused
*		   which main is the "real" main.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for completion (not necessarily program success)
************************************************************/
int main(int argc, char **argv)
{
    rledecode(argc, argv);
    return 0;
}