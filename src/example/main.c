#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>


int main (int argc, const char * argv[]) {
    // create the command line arguments object
    struct cla * cla = CLA_create();

    // add a required named argument with a name and an alias
    CLA_add_required(cla, "--aa", "-a");

    // add a required named argument with a name only
    CLA_add_required(cla, "--bb", nullptr);

    // add a required named argument with an alias only
    CLA_add_required(cla, nullptr, "-c");

    // add an optional named argument with a name and an alias
    CLA_add_optional(cla, "--dd", "-d");

    // add an optional named argument with a name only
    CLA_add_optional(cla, "--ee", nullptr);

    // add an optional named argument with an alias only
    CLA_add_optional(cla, nullptr, "-f");

    // add a flag argument with a name and an alias
    CLA_add_flag(cla, "--gg", "-g");

    // add a flag argument with a name only
    CLA_add_flag(cla, "--hh", nullptr);

    // add a flag argument with an alias only
    CLA_add_flag(cla, nullptr, "-i");

    // specify that the program will have exactly 2 positional arguments
    CLA_add_positionals(cla, 2);

    // parse the arguments that the user provided
    CLA_parse(cla, argc, argv);

    // retrieve value of required named arguments
    fprintf(stdout, "-a/--aa = %s\n", CLA_get_value_required(cla, "-a"));
    fprintf(stdout, "--bb = %s\n", CLA_get_value_required(cla, "--bb"));
    fprintf(stdout, "-c = %s\n", CLA_get_value_required(cla, "-c"));

    // retrieve value of optional named arguments
    if (CLA_has_optional(cla, "-d")) {
        fprintf(stdout, "-d/--dd = %s\n", CLA_get_value_optional(cla, "-d"));
    }

    if (CLA_has_optional(cla, "--ee")) {
        fprintf(stdout, "--ee = %s\n", CLA_get_value_optional(cla, "--ee"));
    }

    if (CLA_has_optional(cla, "-f")) {
        fprintf(stdout, "-f = %s\n", CLA_get_value_optional(cla, "-f"));
    }

    // check flags
    fprintf(stdout, "-g/--gg = %s / %d times\n",
            CLA_has_flag(cla, "-g") ? "used" : "not used",
            CLA_count_flag(cla, "-g"));

    fprintf(stdout, "--hh = %s / %d times\n",
            CLA_has_flag(cla, "--hh") ? "used" : "not used",
            CLA_count_flag(cla, "--hh"));

    fprintf(stdout, "-i = %s / %d times\n",
            CLA_has_flag(cla, "-i") ? "used" : "not used",
            CLA_count_flag(cla, "-i"));

    // print the values of positional arguments provided by the user
    fprintf(stdout, "positional[0] = %s\n", CLA_get_value_positional(cla, 0));
    fprintf(stdout, "positional[1] = %s\n", CLA_get_value_positional(cla, 1));

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
