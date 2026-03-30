#include "cla/cla.h"
#include <stdio.h>
#include <stdlib.h>


static const char * get_basename (const struct cla * cla);
static const char * get_input_filename (const struct cla * cla);
static size_t get_nsamples (const struct cla * cla);
static const char * get_output_filename (const struct cla * cla);
static bool get_verbose (const struct cla * cla);


static const char * get_basename (const struct cla * cla) {
    const char * s = CLA_get_optional_value(cla, "--basename");
    if (s == nullptr) {
        return "sample.";
    }
    return s;
}


static const char * get_input_filename (const struct cla * cla) {
    return CLA_get_positional_value(cla, 0);
}


static size_t get_nsamples (const struct cla * cla) {
    const char * s = CLA_get_required_value(cla, "--nsamples");
    size_t nsamples;
    sscanf(s, " %zu", &nsamples);
    return nsamples;
}


static const char * get_output_filename (const struct cla * cla) {
    return CLA_get_positional_value(cla, 1);
}


static bool get_verbose (const struct cla * cla) {
    return CLA_has_flag(cla, "--verbose");
}


int main (int argc, const char * argv[]) {
    // create the kwargs object
    struct cla * cla = CLA_create();

    // add various types of allowed arguments
    CLA_add_required(cla, "--nsamples", "-s");
    CLA_add_optional(cla, "--basename", "-b");
    CLA_add_flag(cla, "--verbose", "-v");
    CLA_add_positionals(cla, 2);

    // parse the arguments that the user actually chose to use
    CLA_parse(cla, argc, argv);

    // retrieve values
    const char * basename = get_basename(cla);
    const size_t nsamples = get_nsamples(cla);
    const bool verbose = get_verbose(cla);
    const char * input_filename = get_input_filename(cla);
    const char * output_filename = get_output_filename(cla);

    fprintf(stdout, "basename = %s\n", basename);
    fprintf(stdout, "nsamples = %zu\n", nsamples);
    fprintf(stdout, "verbose%sused\n", verbose ? " " : " not ");
    fprintf(stdout, "input filename = %s\n", input_filename);
    fprintf(stdout, "output filename = %s\n", output_filename);

    // free memory resources
    CLA_destroy(&cla);

    // exit
    return EXIT_SUCCESS;
}
