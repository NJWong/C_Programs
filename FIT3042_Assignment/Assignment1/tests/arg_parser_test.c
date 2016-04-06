#include <stdio.h>
#include <string.h>
#include "arg_parser.c"
#include "minunit.h"

int tests_run = 0;

/* handle_arg1() tests */
static char * create_path_test1() {
    char *path = create_path("folder/", "file");
    mu_assert("Error: create_path_test1\n", strcmp(path, "folder/file") == 0);
    return 0;
}

static char * create_path_test2() {
    char *path = create_path("testfolder/", "testfile");
    mu_assert("Error: create_path_test2\n", strcmp(path, "folder/file") == 0);
    return 0;
}

static char * test_all_create_path() {
    mu_run_test(create_path_test1);
    mu_run_test(create_path_test2);
    return 0;
}

static char * test_all_handle_arg1() {
    test_all_create_path();
    return 0;
}

static char * all_tests() {
    test_all_handle_arg1();
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}