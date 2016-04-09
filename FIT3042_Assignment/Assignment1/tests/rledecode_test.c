#include <stdio.h>
#include <string.h>
#include "rledecode.c"
#include "minunit.h"

int tests_run = 0;

/* rledecode tests */

static char * rledecode_test1()
{
    printf("\n***** rledecode_test1 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";

    mu_assert("\nError: rledecode_test1\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * rledecode_test2()
{
    printf("\n***** rledecode_test2 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "-";

    mu_assert("\nError: rledecode_test2\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * rledecode_test3()
{
    printf("\n***** rledecode_test3 LOG *****\n");
    int argc = 2;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";

    mu_assert("\nError: rledecode_test3\n", rledecode(argc, argv) != 0);
    return 0;
}

static char * rledecode_test4()
{
    printf("\n***** rledecode_test4 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "invalid_filename.rle";
    argv[2] = "valid_prefix";

    mu_assert("\nError: rledecode_test4\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * rledecode_test5()
{
    printf("\n***** rledecode_test5 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "invalid_prefix";

    mu_assert("\nError: rledecode_test5\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * rledecode_test6()
{
    printf("\n***** rledecode_test6 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "invalid_filename.rle";
    argv[2] = "invalid_prefix";

    mu_assert("\nError: rledecode_test6\n", rledecode(argc, argv) == 0);
    return 0;
}

static char * all_tests()
{
    mu_run_test(rledecode_test1);
    mu_run_test(rledecode_test2);
    mu_run_test(rledecode_test3);
    mu_run_test(rledecode_test4);
    mu_run_test(rledecode_test5);
    mu_run_test(rledecode_test6);

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