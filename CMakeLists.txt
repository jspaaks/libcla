cmake_minimum_required(VERSION 3.23...3.28)

project(project_cla LANGUAGES C)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/dist")
endif()

add_subdirectory(${PROJECT_ROOT}/man/cla)
add_subdirectory(${PROJECT_ROOT}/src/cla)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # I'm the top-most project
    option(CLA_BUILD_EXAMPLES "Whether to build the libcla examples" ON)
else()
    # I'm not the top-most project, use CLA_BUILD_EXAMPLES as-is
    option(CLA_BUILD_EXAMPLES "Whether to build the libcla examples" OFF)
endif()
if (CLA_BUILD_EXAMPLES)
    add_subdirectory(${PROJECT_ROOT}/src/example)
    add_subdirectory(${PROJECT_ROOT}/src/example_create)
    add_subdirectory(${PROJECT_ROOT}/src/example_flag)
    add_subdirectory(${PROJECT_ROOT}/src/example_optional)
    add_subdirectory(${PROJECT_ROOT}/src/example_positionals)
    add_subdirectory(${PROJECT_ROOT}/src/example_required)
endif()
