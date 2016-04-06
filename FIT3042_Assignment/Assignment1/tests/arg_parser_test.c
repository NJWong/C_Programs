#include <stdio.h>
#include <string.h>
#include "arg_parser.c"
#include "minunit.h"

int tests_run = 0;

/* handle_arg1() tests */
static char * create_path_test1() {
    char *path = create_path("folder/", "file");
    mu_assert("\nError: create_path_test1\n", strcmp(path, "folder/file") == 0);
    return 0;
}

static char * create_path_test2() {
    char *path = create_path("testfolder/", "testfile");
    mu_assert("\nError: create_path_test2\n", strcmp(path, "folder/file") != 0);
    return 0;
}

static char * create_path_test3() {
    mu_assert("\nError: create_path_test3\n", create_path("folder/", NULL) == "NullError");
    return 0;
}

static char * create_path_test4() {
    mu_assert("\nError: create_path_test4\n", create_path(NULL, "file") == "NullError");
    return 0;
}

static char * rleplay_file_exists_test1() {
    char *path = "tests/test1.rle";
    FILE *fp = fopen(path, "ab+");
    fclose(fp);
    mu_assert("\nError: rleplay_file_exists_test1\n", rleplay_file_exists(path) == 1);
    return 0;
}

static char * rleplay_file_exists_test2() {
    char *path = "tests/nothing.rle";
    mu_assert("\nError: rleplay_file_exists_test2\n", rleplay_file_exists(path) != 1);
    return 0;
}

static char * rleplay_file_exists_test3() {
    char *path = NULL;
    mu_assert("\nError: rleplay_file_exists_test3\n", rleplay_file_exists(path) == -1);
    return 0;
}

static char * handle_arg1_test1() {
    char *file = "test2.rle";
    char *path = "rlefiles/test2.rle";
    FILE *f = fopen(path, "ab+");
    fclose(f);
    mu_assert("\nError: rleplay_file_exists_test1\n", handle_arg1(file) == 0);
    return 0;
}

static char * handle_arg1_test2() {
    char *file = "test3.rle";
    mu_assert("\nError: rleplay_file_exists_test2\n", handle_arg1(file) == -1);
    return 0;
}

static char * handle_arg1_test3() {
    return 0;
}

static char * all_tests() {
    mu_run_test(create_path_test1);
    mu_run_test(create_path_test2);
    mu_run_test(create_path_test3);
    mu_run_test(create_path_test4);
    mu_run_test(rleplay_file_exists_test1);
    mu_run_test(rleplay_file_exists_test2);
    mu_run_test(rleplay_file_exists_test3);
    mu_run_test(handle_arg1_test1);
    mu_run_test(handle_arg1_test2);
    mu_run_test(handle_arg1_test3);
    return 0;
}

int main(int argc, char **argv) {
    printf("--- START arg_parser_test ---\n");
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    printf("--- END arg_parser_test ---\n");

    return result != 0;
}