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

Test(CLA_add_positionals, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_add_positionals(nullptr, 0);
}

Test(CLA_add_positionals, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` an initialized `struct cla * self` should pass") {
    CLA_add_positionals(cla, 2);
}

// Test what happens when calling CLA_add_positionals after command line arguments have been parsed

Test(CLA_add_positionals, __LINE__, .exit_code=9, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` valid input after parsing the command line arguments should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_add_positionals(cla, 2);
}

// Tests related to argument `npositionals`

Test(CLA_add_positionals, __LINE__, .exit_code=35, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` a negative integer should fail with the correct exit code") {
    CLA_add_positionals(cla, -1);
}

Test(CLA_add_positionals, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` a `0` should pass") {
    CLA_add_positionals(cla, 0);
}

Test(CLA_add_positionals, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Passing `CLA_add_positionals` a `9` should pass") {
    CLA_add_positionals(cla, 9);
}

Test(CLA_add_positionals, __LINE__, .exit_code=16, .init=setup, .fini=teardown,
    .description="Attempting to call `CLA_add_positionals` twice should fail with the correct exit code") {
    CLA_add_positionals(cla, 2);
    CLA_add_positionals(cla, 2);
}
