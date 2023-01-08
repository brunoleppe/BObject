cmake_minimum_required(VERSION 3.16.1)

project(main)

set(SOURCES main.c)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(blib)

target_link_libraries(${PROJECT_NAME}  blib)
target_compile_options(blib PUBLIC -DB_DEBUG)