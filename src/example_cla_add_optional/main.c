#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    // allocate dynamic memory to store information about what arguments to expect
    struct cla * cla = CLA_create();

    // define a few flags
    CLA_add_optional(cla, "--directory", "-d");
    CLA_add_optional(cla, "--author", nullptr);
    CLA_add_optional(cla, nullptr, "-l");

    // parse the command line arguments and store the result in `cla`
    CLA_parse(cla, argc, argv);

    // print the value of each optional argument, if it was used at all
    if (CLA_has_optional(cla, "--directory")) {
        fprintf(stdout,
                "--directory/--d=%s\n",
                CLA_get_value_optional(cla, "--directory"));
    }

    if (CLA_has_optional(cla, "-l")) {
        fprintf(stdout,
                "-l=%s\n",
                CLA_get_value_optional(cla, "-l"));
    }

    if (CLA_has_optional(cla, "--author")) {
        fprintf(stdout,
                "--author=%s\n",
                CLA_get_value_optional(cla, "--author"));
    }

    // free memory resources
    CLA_destroy(&cla);
    return EXIT_SUCCESS;
}
