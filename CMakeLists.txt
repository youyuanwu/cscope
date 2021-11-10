project(cscope)

cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES
    include/*.h
    src/*.c
)

find_library(ncurses REQUIRED)

add_executable("cscope" ${SOURCES})

target_link_libraries(cscope
ncurses
)
