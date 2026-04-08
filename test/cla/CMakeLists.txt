add_executable(tgt_exe_test_cla)

set_property(TARGET tgt_exe_test_cla PROPERTY OUTPUT_NAME test-cla)

target_compile_definitions(
    tgt_exe_test_cla
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<BOOL:${CLA_BUILD_TESTING_REDIRECT_STDERR}>:-DCLA_BUILD_TESTING_REDIRECT_STDERR>
)

target_compile_features(
    tgt_exe_test_cla
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_test_cla
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_test_cla
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_test_cla
    PRIVATE
        criterion
        tgt_lib_cla
)

target_link_options(
    tgt_exe_test_cla
    PRIVATE
        -pie
)

target_sources(
    tgt_exe_test_cla
    PRIVATE
        test_cla_add_flag.c
        test_cla_add_optional.c
        test_cla_add_positionals.c
        test_cla_add_required.c
        test_cla_count_flag.c
        test_cla_get_exename.c
        test_cla_get_value_optional.c
)

add_test(
    NAME
        test_cla
    COMMAND
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/test-cla   # CMake variables are needed to avoid problems finding the library
)

install(TARGETS tgt_exe_test_cla)
