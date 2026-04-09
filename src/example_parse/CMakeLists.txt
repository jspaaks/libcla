add_executable(tgt_exe_example_parse)

set_property(TARGET tgt_exe_example_parse PROPERTY OUTPUT_NAME example-parse)

target_compile_definitions(
    tgt_exe_example_parse
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_example_parse
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_example_parse
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
    tgt_exe_example_parse
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_example_parse
    PRIVATE
        tgt_lib_cla
)

target_link_options(
    tgt_exe_example_parse
    PRIVATE
        -pie
)

target_sources(
    tgt_exe_example_parse
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_example_parse)
