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

Test(CLA_has_flag, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Passing `CLA_has_flag` an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_has_flag(nullptr, "--aa");
}

Test(CLA_has_flag, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Using `CLA_has_flag` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_has_flag(cla, "--aa");
}

Test(CLA_has_flag, __LINE__, .exit_code=36, .init=setup, .fini=teardown,
    .description="Passing `CLA_has_flag` an uninitialized `name` should fail with the correct exit code") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    CLA_has_flag(cla, nullptr);
}

// This next section is combinatiorial:
//
// calling CLA_has_flag with
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

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with a name to test whether a flag defined by name and alias was used should return true if the flag was used by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with a name to test whether a flag defined by name and alias was used should return true if the flag was used by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with a name to test whether a flag defined by name only was used should return true if the flag was used by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "--aa");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with an alias to test whether a flag defined by name and alias was used should return true if the flag was used by its correct name") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--aa"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "-a");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with an alias to test whether a flag defined by name and alias was used should return true if the flag was used by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "-a");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}

Test(CLA_has_flag, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_has_flag` with an alias to test whether a flag defined by alias only was used should return true if the flag was used by its correct alias") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "-a"
    };
    CLA_add_flag(cla, nullptr, "-a");
    CLA_parse(cla, argc, argv);
    bool expected = true;
    bool actual = CLA_has_flag(cla, "-a");
    cr_assert(expected == actual, "Expected \"%s\" but got \"%s\"\n", expected ? "true" : "false", actual ? "true" : "false");
}
