project(cscope)

cmake_minimum_required(VERSION 3.16)

message("fetching pdcurses")
include(FetchContent)
FetchContent_Declare(pdcurses
  GIT_REPOSITORY https://github.com/youyuanwu/PDCursesMod.git
  GIT_TAG bcd6edda2bb7eecddc27d4ccc690da7232ab9e5b
)
# FetchContent_MakeAvailable(pdcurses)
FetchContent_GetProperties(pdcurses)
if(NOT pdcurses_POPULATED)
  FetchContent_Populate(pdcurses)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${pdcurses_SOURCE_DIR}/cmake")
  add_subdirectory(${pdcurses_SOURCE_DIR} ${pdcurses_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

message("fetching dir")
include(FetchContent)
FetchContent_Declare(dirent
  GIT_REPOSITORY https://github.com/tronkko/dirent.git
  GIT_TAG 1.23.2
)
# FetchContent_MakeAvailable(pdcurses)
FetchContent_GetProperties(dirent)
if(NOT dirent_POPULATED)
  FetchContent_Populate(dirent)
  add_subdirectory(${dirent_SOURCE_DIR} ${dirent_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# for regex use this: https://github.com/laurikari/tre ?

file(GLOB SOURCES
    include/*.h
    src/*.c
    gen/*.h
    gen/*.c
)

# message("finding ncurses")
# find_library(ncurses REQUIRED)
# message("ncurses dir: " ${ncurses_LIBRARY_DIR})

add_executable("cscope" ${SOURCES})

target_include_directories(cscope
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${pdcurses_SOURCE_DIR}
    PRIVATE ${dirent_SOURCE_DIR}/include
)

target_link_libraries(cscope
PUBLIC pdcurses
PUBLIC dirent
)
