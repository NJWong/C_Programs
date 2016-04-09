#include <stdlib.h>
#include <stdio.h>
#include "rledecode.h"

/* Entry point */
int main(int argc, char **argv)
{
    rledecode(argc, argv);

    return 0;
}

void exit_program()
{
    fprintf(stderr, "Exiting Cleanly.\n");
    exit(EXIT_SUCCESS);
}