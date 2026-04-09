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


Test(CLA_get_exename, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_exename` with an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_get_exename(nullptr);
}


Test(CLA_get_exename, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_exename` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_get_exename(cla);
}


Test(CLA_get_exename, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_exename` after valid command line input has been parsed should pass") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_exename(cla);
    char * expected = "exename";
    cr_assert_str_eq(actual, expected);
}
