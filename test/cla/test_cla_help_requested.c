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


Test(CLA_help_requested, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Calling `CLA_help_requested` with an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_help_requested(nullptr);
}


Test(CLA_help_requested, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Calling `CLA_help_requested` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_help_requested(cla);
}


Test(CLA_help_requested, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_help_requested` if the user requested help should return true") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--help"
    };
    CLA_parse(cla, argc, argv);
    bool actual = CLA_help_requested(cla);
    bool expected = true;
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_help_requested, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_help_requested` if the user did not request help should return false") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "value-of-aa"
    };
    CLA_add_optional(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    bool actual = CLA_help_requested(cla);
    bool expected = false;
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}
