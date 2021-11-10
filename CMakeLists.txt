project(cscope)

cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES
    include/*.h
    src/*.c
    gen/*.h
    gen/*.c
)

find_library(ncurses REQUIRED)

add_executable("cscope" ${SOURCES})

target_include_directories(cscope
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(cscope
ncurses
)
