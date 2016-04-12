#include <stdio.h>
#include <string.h>
#include "rledecode.c"
#include "decoder.c"
#include "arg_validator.c"
#include "minunit.h"

int tests_run = 0;

/*********************************
*    VALIDATE ARGUMENTS TESTS    *
*********************************/
static char * validate_args_test1()
{
    printf("\n***** validate_args_test1 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";

    mu_assert("\nError: validate_args_test1\n", validate_args(argc, argv) == 0);
    return 0;
}

static char * validate_args_test2()
{
    printf("\n***** validate_args_test2 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "-";

    mu_assert("\nError: validate_args_test2\n", validate_args(argc, argv) == 0);
    return 0;
}

static char * validate_args_test3()
{
    printf("\n***** validate_args_test3 LOG *****\n");
    int argc = 2;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";

    mu_assert("\nError: validate_args_test3\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test4()
{
    printf("\n***** validate_args_test4 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "invalid_filename.rle";
    argv[2] = "valid_prefix";

    mu_assert("\nError: validate_args_test4\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test5()
{
    printf("\n***** validate_args_test5 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "";

    mu_assert("\nError: validate_args_test5\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test6()
{
    printf("\n***** validate_args_test6 LOG *****\n");
    int argc = 3;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "invalid_filename.rle";
    argv[2] = "";

    mu_assert("\nError: validate_args_test6\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test7()
{
    printf("\n***** validate_args_test7 LOG *****\n");
    int argc = 4;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--scale";

    mu_assert("\nError: validate_args_test7\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test8()
{
    printf("\n***** validate_args_test8 LOG *****\n");
    int argc = 5;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--scale";
    argv[4] = "1";

    mu_assert("\nError: validate_args_test8\n", validate_args(argc, argv) == 0);
    return 0;
}

static char * validate_args_test9()
{
    printf("\n***** validate_args_test9 LOG *****\n");
    int argc = 4;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--tween";

    mu_assert("\nError: validate_args_test9\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test10()
{
    printf("\n***** validate_args_test10 LOG *****\n");
    int argc = 5;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--tween";
    argv[4] = "1";

    mu_assert("\nError: validate_args_test10\n", validate_args(argc, argv) == 0);
    return 0;
}

static char * validate_args_test11()
{
    printf("\n***** validate_args_test11 LOG *****\n");
    int argc = 5;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--scale";
    argv[4] = "-1";

    mu_assert("\nError: validate_args_test11\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * validate_args_test12()
{
    printf("\n***** validate_args_test12 LOG *****\n");
    int argc = 5;
    char* argv[argc + 1];

    argv[0] = "./rledecode";
    argv[1] = "valid_filename.rle";
    argv[2] = "valid_prefix";
    argv[3] = "--tween";
    argv[4] = "-1";

    mu_assert("\nError: validate_args_test12\n", validate_args(argc, argv) != 0);
    return 0;
}

static char * all_tests()
{
    mu_run_test(validate_args_test1);
    mu_run_test(validate_args_test2);
    mu_run_test(validate_args_test3);
    mu_run_test(validate_args_test4);
    mu_run_test(validate_args_test5);
    mu_run_test(validate_args_test6);
    mu_run_test(validate_args_test7);
    mu_run_test(validate_args_test8);
    mu_run_test(validate_args_test9);
    mu_run_test(validate_args_test10);
    mu_run_test(validate_args_test11);
    mu_run_test(validate_args_test12);
    return 0;
}

int main(int argc, char **argv)
{
    printf("--- START rledecode_test ---\n");
    
    FILE *fp = fopen("valid_filename.rle", "ab+");
    fclose(fp);

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