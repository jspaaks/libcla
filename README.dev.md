# Developer notes

## CMake

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build

# generate the build files
$ cmake ..

# build the project
$ cmake --build .

# install the project to <repo>/build/cmake/dist
$ cmake --install .

# checkout man pages, e.g.
$ man -l ./dist/share/man/man3/CLA.3
```

The CMake variable `CLA_BUILD_EXAMPLES` can be used to control whether the examples get built.
If this project is the top project, the default is to build the examples; conversely, if CLA is
used as a dependency of a different project, the default is to not build the examples. Either way,
building can be controlled by explicitly setting the beforementioned variable, e.g like so:

```console
cmake -DCLA_BUILD_EXAMPLES=ON ..
```

### CMake variables

| CMake variable                     | Description                            |
|---                                 | ---                                    |
|`CLA_BUILD_EXAMPLES`                | Build the CLA examples                 |
|`CLA_BUILD_TESTING_REDIRECT_STDERR` | Redirect stderr in the tests           |
|`CLA_BUILD_TESTING`                 | Build the CLA tests                    |

## Testing

Building and running the tests require that [Criterion](https://github.com/Snaipe/Criterion) is installed on the system, e.g. with

```console
$ sudo apt install libcriterion-dev
```

Run the tests with

```console
# run all tests verbosely, and using one concurrent job
$ ./dist/bin/test-cla -j1 --verbose
# run only tests from the CLA_add_optional suite
$ ./dist/bin/test-cla -j1 --verbose --filter CLA_add_optional/*
# run only the test from line 99 from the CLA_add_optional suite
$ ./dist/bin/test-cla -j1 --verbose --filter CLA_add_optional/99
```

Or simply,

```console
$ ctest
```
