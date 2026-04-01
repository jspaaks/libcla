#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>


int main (int argc, const char * argv[]) {
    // create the command line arguments object
    struct cla * cla = CLA_create();

    // add a required argument with a longname and a shortname
    CLA_add_required(cla, "--aa", "-a");

    // add a required argument with a longname only
    CLA_add_required(cla, "--bb", nullptr);

    // add a required argument with a shortname only
    CLA_add_required(cla, nullptr, "-c");

    // add an optional argument with a longname and a shortname
    CLA_add_optional(cla, "--dd", "-d");

    // add an optional argument with a longname only
    CLA_add_optional(cla, "--ee", nullptr);

    // add an optional argument with a shortname only
    CLA_add_optional(cla, nullptr, "-f");

    // add a flag argument with a longname and a shortname
    CLA_add_flag(cla, "--gg", "-g");

    // add a flag argument with a longname only
    CLA_add_flag(cla, "--hh", nullptr);

    // add a flag argument with a shortname only
    CLA_add_flag(cla, nullptr, "-i");

    // specify that the program will have exactly 2 positional arguments
    CLA_add_positionals(cla, 2);

    // parse the arguments that the user actually chose to use
    CLA_parse(cla, argc, argv);

    // retrieve value of required options
    fprintf(stdout, "-a/--aa = %s\n", CLA_get_value_required(cla, "-a"));
    fprintf(stdout, "--bb = %s\n", CLA_get_value_required(cla, "--bb"));
    fprintf(stdout, "-c = %s\n", CLA_get_value_required(cla, "-c"));

    // retrieve value of optional arguments
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

    fprintf(stdout, "positional[0] = %s\n", CLA_get_value_positional(cla, 0));
    fprintf(stdout, "positional[1] = %s\n", CLA_get_value_positional(cla, 1));

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
