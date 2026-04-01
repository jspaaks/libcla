#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    // allocate dynamic memory to store information about
    // what arguments to expect
    struct cla * cla = CLA_create();

    // specify that our program will have exactly 2 positional arguments
    CLA_add_positionals(cla, 2);

    // parse the command line arguments and store the result in `cla`
    CLA_parse(cla, argc, argv);

    // print the value of each positional argument
    fprintf(stdout,
           "first positional=%s\n",
           CLA_get_value_positional(cla, 0));

    fprintf(stdout,
           "second positional=%s\n",
           CLA_get_value_positional(cla, 1));

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
