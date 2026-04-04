#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    // allocate dynamic memory to store information about
    // what arguments to expect
    struct cla * cla = CLA_create();

    // define a few required named arguments
    CLA_add_required(cla, "--nsamples", "-n");
    CLA_add_required(cla, "--freq", nullptr);
    CLA_add_required(cla, nullptr, "-f");

    // parse the command line arguments and store the result in `cla`
    CLA_parse(cla, argc, argv);

    // handle help requests
    if (CLA_help_requested(cla)) {
        fprintf(stdout, "Valid option names:\n"
                        "    --nsamples, -n\n"
                        "    --freq\n"
                        "    -f\n");
        exit(EXIT_SUCCESS);
    }

    // print the value of each required named argument
    fprintf(stdout,
           "--nsamples/-n=%s\n",
           CLA_get_value_required(cla, "--nsamples"));

    fprintf(stdout,
           "--freq=%s\n",
           CLA_get_value_required(cla, "--freq"));

    fprintf(stdout,
           "-f=%s\n",
           CLA_get_value_required(cla, "-f"));

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
