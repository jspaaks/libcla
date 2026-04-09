#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, const char * argv[]) {
    // create the command line arguments object
    struct cla * cla = CLA_create();

    // register a required named argument with a name and an alias
    CLA_add_required(cla, "--aa", "-a");

    // register an optional named argument with a name and an alias
    CLA_add_optional(cla, "--bb", "-b");

    // register a flag argument with a name and an alias
    CLA_add_flag(cla, "--cc", "-c");

    // specify that the program will have exactly 2 positional arguments
    CLA_add_positionals(cla, 2);

    // parse the arguments that the user provided
    CLA_parse(cla, argc, argv);

    // handle help requests
    if (CLA_help_requested(cla)) {
        fprintf(stdout, "Usage: example-parse REQUIREDS [OPTIONALS] [FLAGS] POS0 POS1\n"
                        "\n"
                        "  Requireds:\n"
                        "    --aa, -a <VAL>     ... description ...\n"
                        "\n"
                        "  Optionals:\n"
                        "    --bb, -b <VAL>     ... description ...\n"
                        "\n"
                        "  Flags:\n"
                        "    --cc, -c           ... description ...\n"
                        "\n"
                        "  Positionals:\n"
                        "    POS0               ... description ...\n"
                        "    POS1               ... description ...\n"
                        "\n");
        exit(EXIT_SUCCESS);
    }

    // show how the program understood each of the user-provided arguments
    CLA_parsed_as(cla, stdout);

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
