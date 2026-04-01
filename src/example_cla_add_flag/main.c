#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    // allocate dynamic memory to store information about what arguments to expect
    struct cla * cla = CLA_create();

    // define a few flags
    CLA_add_flag(cla, "--directory", "-d");
    CLA_add_flag(cla, "--author", nullptr);
    CLA_add_flag(cla, nullptr, "-l");

    // parse the command line arguments and store the result in `cla`
    CLA_parse(cla, argc, argv);

    // print how many times each flag was used, if it was used at all
    if (CLA_has_flag(cla, "--directory")) {
        fprintf(stdout,
                "--directory/--d was used %d times\n",
                CLA_count_flag(cla, "--directory"));
    }

    if (CLA_has_flag(cla, "-l")) {
        fprintf(stdout,
                "-l was used %d times\n",
                CLA_count_flag(cla, "-l"));
    }

    if (CLA_has_flag(cla, "--author")) {
        fprintf(stdout,
                "--author was used %d times\n",
                CLA_count_flag(cla, "--author"));
    }

    // free memory resources
    CLA_destroy(&cla);
    return EXIT_SUCCESS;
}
