#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    // allocate dynamic memory to store information about
    // what arguments to expect
    struct cla * cla = CLA_create();

    // specify that the program will have exactly 2
    // positional arguments
    CLA_add_positionals(cla, 2);

    // parse the arguments that the user provided
    CLA_parse(cla, argc, argv);

    // print the value of the first positional
    fprintf(stdout, "positional[0] = %s\n",
            CLA_get_value_positional(cla, 0));

    // print the value of the second positional
    fprintf(stdout, "positional[1] = %s\n",
            CLA_get_value_positional(cla, 1));

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
