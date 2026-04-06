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

Test(CLA_add_required, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_add_required(nullptr, "--sample", "-s");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an initialized `struct cla * self` should pass") {
    CLA_add_required(cla, "--sample", "-s");
}

// Test what happens when calling CLA_add_required after command line arguments have been parsed

Test(CLA_add_required, __LINE__, .exit_code=9, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` valid input after parsing the command line arguments should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_add_required(cla, "--sample", "-s");
}

// Tests related to combinations of arguments `name` and `alias`

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name but no alias should pass") {
    CLA_add_required(cla, "--sample", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias but no name should pass") {
    CLA_add_required(cla, nullptr, "-s");
}

Test(CLA_add_required, __LINE__, .exit_code=10, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` neither a name nor an alias should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, nullptr);
}

// Tests related to argument `name`

Test(CLA_add_required, __LINE__, .exit_code=33, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name `--help` should fail with the correct exit code") {
    CLA_add_required(cla, "--help", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=14, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name that is too short should fail with the correct exit code") {
    CLA_add_required(cla, "--a", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the shortest allowed name should pass") {
    CLA_add_required(cla, "--aa", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=15, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name that is too long should fail with the correct exit code") {
    CLA_add_required(cla, "--aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffffggg", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the longest allowed name should pass") {
    CLA_add_required(cla, "--aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffffgg", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=17, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name whose 1st character isn't `-` should fail with the correct exit code") {
    CLA_add_required(cla, "aaaa", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=17, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name whose 2nd character isn't `-` should fail with the correct exit code") {
    CLA_add_required(cla, "-aaa", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a valid name should pass") {
    CLA_add_required(cla, "--aa", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a capitalized name should pass") {
    CLA_add_required(cla, "--AA", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name consisting only of numbers should pass") {
    CLA_add_required(cla, "--12", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=18, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a name containing non-alphanumeric characters should fail with the correct exit code") {
    CLA_add_required(cla, "--aA!", nullptr);
}

Test(CLA_add_required, __LINE__, .exit_code=19, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the same name twice should fail with the correct exit code") {
    CLA_add_required(cla, "--aa", nullptr);
    CLA_add_required(cla, "--aa", nullptr);
}

// Tests related to argument `alias`

Test(CLA_add_required, __LINE__, .exit_code=32, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias `-h` should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "-h");
}

Test(CLA_add_required, __LINE__, .exit_code=4, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias that is too short should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "-");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the shortest allowed alias should pass") {
    CLA_add_required(cla, nullptr, "-a");
}

Test(CLA_add_required, __LINE__, .exit_code=4, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias that is too long should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "-aa");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the longest allowed alias should pass") {
    CLA_add_required(cla, nullptr, "-a");
}

Test(CLA_add_required, __LINE__, .exit_code=5, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias that doesn't start with `-` should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "aa");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a valid alias should pass") {
    CLA_add_required(cla, nullptr, "-a");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` a capitalized alias should pass") {
    CLA_add_required(cla, nullptr, "-A");
}

Test(CLA_add_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias consisting only of a number should pass") {
    CLA_add_required(cla, nullptr, "-1");
}

Test(CLA_add_required, __LINE__, .exit_code=6, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` an alias containing non-alphanumeric characters should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "-!");
}

Test(CLA_add_required, __LINE__, .exit_code=7, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_required` the same alias twice should fail with the correct exit code") {
    CLA_add_required(cla, nullptr, "-a");
    CLA_add_required(cla, nullptr, "-a");
}
