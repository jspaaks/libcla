#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>


int main (int argc, const char * argv[]) {
    // create the command line arguments object
    struct cla * cla = CLA_create();

    // add a required named argument with a name and an alias
    CLA_add_required(cla, "--aa", "-a");

    // add an optional named argument with a name and an alias
    CLA_add_optional(cla, "--bb", "-b");

    // add a flag argument with a name and an alias
    CLA_add_flag(cla, "--cc", "-c");

    // specify that the program will have exactly 1
    // positional argument
    CLA_add_positionals(cla, 1);

    // parse the arguments that the user provided
    CLA_parse(cla, argc, argv);

    // show how the program understood each of the user-provided arguments
    CLA_parsed_as(cla, stdout);

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
