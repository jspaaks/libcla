#include "cla/cla.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>   // EXIT_SUCCESS

static void setup (void);
static void teardown (void);
static struct cla * cla = nullptr;

static void setup (void) {
    cla = CLA_create();
#ifdef CLA_BUILD_TESTING_REDIRECT_STDERR
    // avoid printing the stderr messages (make sure to catch exit codes in the tests though)
    cr_redirect_stderr();
#endif // CLA_BUILD_TESTING_REDIRECT_STDERR
}

static void teardown (void) {
    CLA_destroy(&cla);
}

Test(CLA_count_flag, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Passing `CLA_count_flag` an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_count_flag(nullptr, "--aa");
}

Test(CLA_count_flag, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_count_flag(cla, "--aa");
}

Test(CLA_count_flag, __LINE__, .exit_code=36, .init=setup, .fini=teardown,
    .description="Passing `CLA_count_flag` an uninitialized `name` should fail with the correct exit code") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    CLA_count_flag(cla, nullptr);
}

// This next section is combinatiorial for the case where the user used one argument:
//
// counting by
//    - name
//    - alias
//
// flag definitions
//    - name and alias
//    - name only
//    - alias only
//
// using
//    - correct name only
//    - correct alias only

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name and alias should return 1 if the flag was used once by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name and alias should return 1 if the flag was used once by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name only should return 1 if the flag was used once by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by name and alias should return 1 if the flag was used once by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by name and alias should return 1 if the flag was used once by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by alias only should return 1 if the flag was used once by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, nullptr, "-a");
    CLA_parse(cla, argc, argv);
    int expected = 1;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

// This next section is combinatiorial for the case where the user used two arguments:
//
// counting by
//    - name
//    - alias
//
// flag definitions
//    - name and alias
//    - name only
//    - alias only
//
// using
//    - correct name only
//    - correct alias only
//    - mixture of correct name and correct alias

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name and alias should return 2 if the flag was used twice by its correct name") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name and alias should return 2 if the flag was used twice by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name and alias should return 2 if the flag was used once by its correct name and once by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by name occurrences of a flag defined by name only should return 2 if the flag was used twice by its correct name") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by name and alias should return 2 if the flag was used twice by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by name and alias should return 2 if the flag was used once by its correct name and once by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}

Test(CLA_count_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Using `CLA_count_flag` to count by alias occurrences of a flag defined by alias only should return 2 if the flag was used twice by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "-a"
    };
    CLA_add_flag(cla, nullptr, "-a");
    CLA_parse(cla, argc, argv);
    int expected = 2;
    int actual = CLA_count_flag(cla, "-a");
    cr_assert(expected == actual, "Expected %d but got %d\n", expected, actual);
}
