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


Test(CLA_get_value_required, __LINE__, .exit_code=21, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with an uninitialized `struct cla * self` should fail with the correct exit code") {
    CLA_get_value_required(nullptr, nullptr);
}

Test(CLA_get_value_required, __LINE__, .exit_code=36, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with an uninitialized `name` should fail with the correct exit code") {
    CLA_get_value_required(cla, nullptr);
}

Test(CLA_get_value_required, __LINE__, .exit_code=8, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` while the command line arguments have not been parsed yet should fail with the correct exit code") {
    CLA_get_value_required(cla, "--aa");
}

Test(CLA_get_value_required, __LINE__, .exit_code=34, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` while help has been requested should fail with the correct exit code") {
    int argc = 2;
    const char * argv[] = {
        "exename",
        "--help"
    };
    CLA_add_required(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    CLA_get_value_required(cla, "--aa");
}

Test(CLA_get_value_required, __LINE__, .exit_code=11, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with unknown named argument should fail with the correct exit code") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_parse(cla, argc, argv);
    CLA_get_value_required(cla, "--aa");
}

Test(CLA_get_value_required, __LINE__, .exit_code=12, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with valid optional named argument should fail with the correct exit code") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "value-of-aa"
    };
    CLA_add_optional(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    CLA_get_value_required(cla, "--aa");
}

Test(CLA_get_value_required, __LINE__, .exit_code=20, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with valid required named argument should fail with the correct exit code if the argument isn't used") {
    int argc = 1;
    const char * argv[] = {
        "exename"
    };
    CLA_add_required(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    CLA_get_value_required(cla, "--aa");
}

// This next section is combinatorial:
// 
// called with
// - name
// - alias
// 
// defined with 
// - name and alias
// - name only
// - alias only
// 
// args have
// - name
// - alias

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with a name to retrieve the value of a required named argument defined by name and alias should return the named argument's value if the argument was used by its correct name") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "value-of-aa"
    };
    CLA_add_required(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "--aa");
    char * expected = "value-of-aa";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with a name to retrieve the value of a required named argument defined by name and alias should return the named argument's value if the argument was used by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "value-of-a"
    };
    CLA_add_required(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "--aa");
    char * expected = "value-of-a";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with a name to retrieve the value of a required named argument defined by name only should return the named argument's value if the argument was used by its correct name") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "value-of-aa"
    };
    CLA_add_required(cla, "--aa", nullptr);
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "--aa");
    char * expected = "value-of-aa";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with an alias to retrieve the value of a required named argument defined by name and alias should return the named argument's value if the argument was used by its correct name") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "--aa",
        "value-of-aa"
    };
    CLA_add_required(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "-a");
    char * expected = "value-of-aa";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with an alias to retrieve the value of a required named argument defined by name and alias should return the named argument's value if the argument was used by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "value-of-a"
    };
    CLA_add_required(cla, "--aa", "-a");
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "-a");
    char * expected = "value-of-a";
    cr_assert_str_eq(actual, expected);
}

Test(CLA_get_value_required, __LINE__, .exit_code=EXIT_SUCCESS, .init=setup, .fini=teardown,
    .description="Calling `CLA_get_value_required` with an alias to retrieve the value of a required named argument defined by alias only should return the named argument's value if the argument was used by its correct alias") {
    int argc = 3;
    const char * argv[] = {
        "exename",
        "-a",
        "value-of-a"
    };
    CLA_add_required(cla, nullptr, "-a");
    CLA_parse(cla, argc, argv);
    char * actual = (char *) CLA_get_value_required(cla, "-a");
    char * expected = "value-of-a";
    cr_assert_str_eq(actual, expected);
}
