add_executable(tgt_exe_example_required)

set_property(TARGET tgt_exe_example_required PROPERTY OUTPUT_NAME example-required)

target_compile_definitions(
    tgt_exe_example_required
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_example_required
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_example_required
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
    tgt_exe_example_required
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_example_required
    PRIVATE
        tgt_lib_cla
)

target_link_options(
    tgt_exe_example_required
    PRIVATE
        -pie
)

target_sources(
    tgt_exe_example_required
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_example_required)
