# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 3.25.1)
project (bogen)

set(SOURCE_FILES 
    helper_file.c
    helper_string.c
    template.c
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})