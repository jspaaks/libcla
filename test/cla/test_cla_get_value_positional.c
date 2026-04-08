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


Test(CLA_get_value_positional, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` with an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_get_value_positional(nullptr, 0);
}

Test(CLA_get_value_positional, __LINE__, .exit_code=25, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` with an index that is too small should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_get_value_positional(cla, -1);
}

Test(CLA_get_value_positional, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` with a valid index should pass") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "positional-value-0"
    };
    CLA_add_positionals(cla, 1);
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_positional(cla, 0);
    char * expected = "positional-value-0";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_positional, __LINE__, .exit_code=26, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` with an index that is too large should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_get_value_positional(cla, 0);
}

Test(CLA_get_value_positional, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_get_value_positional(cla, 0);
}

Test(CLA_get_value_positional, __LINE__, .exit_code=34, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_positional` while help has been requested should fail with the correct exit code") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--help"
    };
    CLA_add_positionals(cla, 1);
    CLA_parse(cla, argc, argv);
    CLA_get_value_positional(cla, 0);
}
