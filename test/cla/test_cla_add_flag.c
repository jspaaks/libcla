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

// Tests related to argument `self`

Test(CLA_add_flag, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_add_flag(nullptr, "--sample", "-s");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an initialized `struct cla * self` should pass") {
    CLA_add_flag(cla, "--sample", "-s");
}

// Test what happens when calling CLA_add_flag after command line arguments have been parsed

Test(CLA_add_flag, __LINE__, .exit_code=9, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` valid input after parsing the command line arguments should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_add_flag(cla, "--sample", "-s");
}

// Tests related to combinations of arguments `name` and `alias`

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name but no alias should pass") {
    CLA_add_flag(cla, "--sample", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias but no name should pass") {
    CLA_add_flag(cla, nullptr, "-s");
}

Test(CLA_add_flag, __LINE__, .exit_code=10, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` neither a name nor an alias should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, nullptr);
}

// Tests related to argument `name`

Test(CLA_add_flag, __LINE__, .exit_code=33, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name `--help` should fail with the correct exit code") {
    CLA_add_flag(cla, "--help", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=14, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name that is too short should fail with the correct exit code") {
    CLA_add_flag(cla, "--a", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the shortest allowed name should pass") {
    CLA_add_flag(cla, "--aa", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=15, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name that is too long should fail with the correct exit code") {
    CLA_add_flag(cla, "--aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffffggg", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the longest allowed name should pass") {
    CLA_add_flag(cla, "--aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffffgg", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=17, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name whose 1st character isn't `-` should fail with the correct exit code") {
    CLA_add_flag(cla, "aaaa", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=17, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name whose 2nd character isn't `-` should fail with the correct exit code") {
    CLA_add_flag(cla, "-aaa", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a valid name should pass") {
    CLA_add_flag(cla, "--aa", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a capitalized name should pass") {
    CLA_add_flag(cla, "--AA", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name consisting only of numbers should pass") {
    CLA_add_flag(cla, "--12", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=18, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a name containing non-alphanumeric characters should fail with the correct exit code") {
    CLA_add_flag(cla, "--aA!", nullptr);
}

Test(CLA_add_flag, __LINE__, .exit_code=19, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the same name twice should fail with the correct exit code") {
    CLA_add_flag(cla, "--aa", nullptr);
    CLA_add_flag(cla, "--aa", nullptr);
}

// Tests related to argument `alias`

Test(CLA_add_flag, __LINE__, .exit_code=32, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias `-h` should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "-h");
}

Test(CLA_add_flag, __LINE__, .exit_code=4, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias that is too short should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "-");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the shortest allowed alias should pass") {
    CLA_add_flag(cla, nullptr, "-a");
}

Test(CLA_add_flag, __LINE__, .exit_code=4, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias that is too long should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "-aa");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the longest allowed alias should pass") {
    CLA_add_flag(cla, nullptr, "-a");
}

Test(CLA_add_flag, __LINE__, .exit_code=5, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias that doesn't start with `-` should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "aa");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a valid alias should pass") {
    CLA_add_flag(cla, nullptr, "-a");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` a capitalized alias should pass") {
    CLA_add_flag(cla, nullptr, "-A");
}

Test(CLA_add_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias consisting only of a number should pass") {
    CLA_add_flag(cla, nullptr, "-1");
}

Test(CLA_add_flag, __LINE__, .exit_code=6, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` an alias containing non-alphanumeric characters should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "-!");
}

Test(CLA_add_flag, __LINE__, .exit_code=7, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_flag` the same alias twice should fail with the correct exit code") {
    CLA_add_flag(cla, nullptr, "-a");
    CLA_add_flag(cla, nullptr, "-a");
}

