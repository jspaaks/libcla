add_library(tgt_lib_cla SHARED)

set_property(TARGET tgt_lib_cla PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET tgt_lib_cla PROPERTY OUTPUT_NAME cla)

generate_export_header(
    tgt_lib_cla
    BASE_NAME cla
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/cla/export.h
)

target_compile_definitions(
    tgt_lib_cla
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_lib_cla
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_lib_cla
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_lib_cla
    PRIVATE
        ${PROJECT_ROOT}/include
)

target_sources(
    tgt_lib_cla
    PRIVATE
        cla.c
    PUBLIC
        FILE_SET fset_lib_cla_headers
            TYPE HEADERS
            BASE_DIRS
                ../../include
                ${CMAKE_BINARY_DIR}/include  # contains cmake-generated file
            FILES
                ../../include/cla/cla.h
                ${CMAKE_BINARY_DIR}/include/cla/export.h  # cmake-generated file
)

install(TARGETS tgt_lib_cla
    LIBRARY
    FILE_SET fset_lib_cla_headers
)
