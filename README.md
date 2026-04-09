# libcla

C library for parsing user-provided command line arguments. Supported argument types are:

1. positional arguments
2. required named arguments
3. optional named arguments
4. flags

For instructions on building, see [`README.dev.md`](README.dev.md).

## Executables

- `example-cla`: Example program that illustrates intended usage of the whole library.
- `example-flag`: Example program that illustrates intended usage of flags (`CLA_add_flag`, `CLA_count_flag`, `CLA_has_flag`).
- `example-optional`: Example program that illustrates intended usage of optional named arguments  (`CLA_add_optional`, `CLA_get_value_optional`, `CLA_has_optional`).
- `example-parse`: Example program that illustrates parsing (`CLA_parse`, `CLA_parsed_as`).
- `example-positionals`: Example program that illustrates intended usage of positional arguments  (`CLA_add_positionals`, `CLA_get_value_positional`).
- `example-required`: Example program that illustrates intended usage of required named arguments  (`CLA_add_required`, `CLA_get_value_required`).

## Libraries

- `cla`: Library for handling of command line arguments.

## `man` pages

- `CLA.3`
- `CLA_add_flag.3`
- `CLA_add_optional.3`
- `CLA_add_positionals.3`
- `CLA_add_required.3`
- `CLA_count_flag.3`
- `CLA_create.3`
- `CLA_destroy.3`
- `CLA_get_exename.3`
- `CLA_get_value_optional.3`
- `CLA_get_value_positional.3`
- `CLA_get_value_required.3`
- `CLA_has_flag.3`
- `CLA_has_optional.3`
- `CLA_help_requested.3`
- `CLA_parse.3`
- `CLA_parsed_as.3`

## Example

`example-parse` is an example program for the usage of the library. Its source code is available in
[`src/example_parse/main.c`](src/example_parse/main.c) and is reproduced below for convenience:

```c
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
```

Let's see how that works in practice:

```console
$ ./example-parse
ERROR 30: Expected at least 2 positional arguments, aborting.
```

OK, so add a value for the positional arguments:

```console
$ ./example-parse pos0 pos1
ERROR 20: Required key '-a/--aa' not found, aborting.
```

Makes sense, our program above expects an argument named `-a` or `--aa`; let's add it then:

```console
$ ./example-parse -a pos0 pos1
ERROR 31: '-a' requires a value but none given or not all positionals have been defined, aborting.
```

Ah yes, `-a` is a key-value pair so it needs a value:

```console
$ ./example-parse -a value-of-a pos0 pos1
User-provided arguments were parsed as follows:
exename    ./example-parse
required   -a
value      value-of-a
positional pos0
positional pos1
```

It works! But we haven't tried with the optional arguments yet. Let's see what options we have:

```console
$ ./example-parse --help
Usage: example-parse REQUIREDS [OPTIONALS] [FLAGS] POS0 POS1

  Requireds:
    --aa, -a <VAL>     ... description ...

  Optionals:
    --bb, -b <VAL>     ... description ...

  Flags:
    --cc, -c           ... description ...

  Positionals:
    POS0               ... description ...
    POS1               ... description ...

```

With optional named argument `--bb/-b` and flag `--cc/-c`:

```console
$ ./example-parse -a value-of-a -b value-of-b -c --cc pos0 pos1
User-provided arguments were parsed as follows:
exename    ./example-parse
required   -a
value      value-of-a
optional   -b
value      value-of-b
flag       -c
flag       --cc
positional pos0
positional pos1
```

Note that flags may be repeated, but required and optional named arguments can only appear once.

## Alternatives

There are many alternatives out there for parsing command line arguments. Here are some:

1. https://github.com/cofyc/argparse?tab=readme-ov-file
1. https://www.argtable.org/
1. https://likle.github.io/cargs/
1. https://michaeldipperstein.github.io/optlist.html
1. https://www.dmulholl.com/docs/args/master/api.html

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier)
and the [Copier template for C projects](https://github.com/jspaaks/copier-template-for-c-projects)._
