#include <stdio.h>
#include <string.h>
#include <rledecode.c>
#include "minunit.h"

int tests_run = 0;

/* rledecode tests */

static char * rledecode_test1()
{
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "filename.rle";
    argv[2] = "prefix";

    mu_assert("\nError: rledecode_test1\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * all_tests()
{
    mu_run_test(rledecode_test1);

    return 0;
}

int main(int argc, char **argv)
{
    printf("--- START rledecode_test ---\n");
    char *result = all_tests();
    if (result != 0)
    {
        printf("\nTEST FAIL");
        printf("%s\n", result);
    }
    else
    {
        printf("\nALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);
    printf("--- END rledecode_test ---\n");

    return result != 0;
}