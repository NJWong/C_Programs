#include <stdio.h>
#include <string.h>
#include "arg_parser.c"
#include "minunit.h"

int tests_run = 0;

/* handle_arg1() tests */

static char * rleplay_file_exists_test1()
{
    char *path = "tests/test1.rle";
    FILE *fp = fopen(path, "ab+");
    fclose(fp);
    mu_assert("\nError: rleplay_file_exists_test1\n", rleplay_file_exists(path) == 1);
    return 0;
}

static char * rleplay_file_exists_test2()
{
    char *path = "tests/nothing.rle";
    mu_assert("\nError: rleplay_file_exists_test2\n", rleplay_file_exists(path) != 1);
    return 0;
}

static char * rleplay_file_exists_test3()
{
    char *path = NULL;
    mu_assert("\nError: rleplay_file_exists_test3\n", rleplay_file_exists(path) == -1);
    return 0;
}

static char * handle_arg1_test1()
{
    char *filepath = "test2.rle";
    FILE *f = fopen(filepath, "ab+");
    fclose(f);
    mu_assert("\nError: handle_arg1_test1\n", handle_arg1(filepath) == 0);
    return 0;
}

static char * handle_arg1_test2()
{
    char *filepath = "test3.rle";
    mu_assert("\nError: handle_arg1_test2\n", handle_arg1(filepath) != 0);
    return 0;
}

/* handle_arg2 tests */
static char * is_valid_prefix_test1()
{
    char *prefix = "testprefix";
    mu_assert("\nError: is_valid_prefix_test1\n", is_valid_prefix(prefix) == 1);
    return 0;
}

static char * is_valid_prefix_test2()
{
    char *prefix = "";
    mu_assert("\nError: is_valid_prefix_test2\n", is_valid_prefix(prefix) == 0);
    return 0;
}

static char * handle_arg2_test1()
{
    char *arg2 = "-";
    mu_assert("\nError: handle_arg2_test1\n", handle_arg2(arg2) == 1);
    return 0;
}

static char * handle_arg2_test2()
{
    char *arg2 = "testprefix";
    mu_assert("\nError: handle_arg2_test2\n", handle_arg2(arg2) == 2);
    return 0;
}

static char * handle_arg2_test3()
{
    char *arg2 = "";
    mu_assert("\nError: handle_arg2_test3\n", handle_arg2(arg2) == -1);
    return 0;
}

static char * parse_arg_test1() {
    char *filename = "test7.rle";
    FILE *f = fopen("test7.rle", "ab+");
    fclose(f);

    mu_assert("\nError: parse_arg_test1\n", parse_arg(1, filename) == 0);
    return 0;
}

static char * parse_arg_test2() {
    char *filename = "nothing.rle";

    mu_assert("\nError: parse_arg_test2\n", parse_arg(1, filename) == -1);
    return 0;
}

static char * parse_arg_test3() {
    char *filename = "";

    mu_assert("\nError: parse_arg_test3\n", parse_arg(1, filename) == -1);
    return 0;
}

static char * parse_arg_test4() {
    char *filename = NULL;

    mu_assert("\nError: parse_arg_test4\n", parse_arg(1, filename) == 0);
    return 0;
}

static char * all_tests()
{
    mu_run_test(rleplay_file_exists_test1);
    mu_run_test(rleplay_file_exists_test2);
    mu_run_test(rleplay_file_exists_test3);
    mu_run_test(handle_arg1_test1);
    mu_run_test(handle_arg1_test2);
    mu_run_test(is_valid_prefix_test1);
    mu_run_test(is_valid_prefix_test2);
    mu_run_test(handle_arg2_test1);
    mu_run_test(handle_arg2_test2);
    mu_run_test(handle_arg2_test3);
    mu_run_test(parse_arg_test1);
    mu_run_test(parse_arg_test2);
    mu_run_test(parse_arg_test3);
    mu_run_test(parse_arg_test4);

    return 0;
}

int main(int argc, char **argv)
{
    printf("--- START arg_parser_test ---\n");
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
    printf("--- END arg_parser_test ---\n");

    return result != 0;
}