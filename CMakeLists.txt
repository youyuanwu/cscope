project(cscope)

cmake_minimum_required(VERSION 3.16)

# bison targets
find_package(BISON)
if(NOT BISON_FOUND)
    message(FATAL_ERROR "bison not found")
endif()
BISON_TARGET(egrep ${cscope_SOURCE_DIR}/src/egrep.y ${cscope_SOURCE_DIR}/gen/egrep.c)
if(NOT BISON_egrep_DEFINED)
message(FATAL_ERROR "bison failed gen")
endif()

# flex targets
find_package(FLEX)
if(NOT FLEX_FOUND)
    message(FATAL_ERROR "flex not found")
endif()
FLEX_TARGET(fscanner ${cscope_SOURCE_DIR}/src/fscanner.l ${cscope_SOURCE_DIR}/gen/fscanner.c)

# assume stdc lib is present

# configure file
find_package(Curses)
if(CURSES_FOUND)
    if(CURSES_HAVE_CURSES_H)
        set(HAS_CURSES 1)
    endif()
    if(CURSES_HAVE_NCURSES_H)
        message("Found ncurses")
        set(USE_NCURSES 1)
    endif()
else()
    message(FATAL_ERROR "cursers lib not found.")
endif()

include(CheckFunctionExists)
check_function_exists(asprintf HAVE_ASPRINTF)
check_function_exists(lstat HAVE_LSTAT)
# sigsetjmp detection is not working correctly
include(CheckSymbolExists)
check_symbol_exists(sigsetjmp "setjmp.h" HAVE_SIGSETJMP)

include(CheckIncludeFile)
check_include_file(dirent.h HAVE_DIRENT_H)
if(NOT ${HAVE_DIRENT_H})
    message(FATAL_ERROR "dirent.h not found.")
endif()

check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(getopt.h HAVE_GETOPT_LONG)
check_include_file(inttypes.h HAVE_INTTYPES_H)
set(cscope_CONFIG_H_OUTPUT ${cscope_SOURCE_DIR}/gen/config.h)
configure_file(${cscope_SOURCE_DIR}/cmakeconfig.h.in ${cscope_CONFIG_H_OUTPUT} @ONLY)

file(GLOB SOURCES
    include/*.h
    src/*.c
)

add_executable("cscope"
    ${SOURCES} 
    ${BISON_egrep_OUTPUTS}
    ${FLEX_fscanner_OUTPUTS}
    ${cscope_CONFIG_H_OUTPUT})

target_include_directories(cscope
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gen
)

target_link_libraries(cscope
    PRIVATE ${CURSES_LIBRARIES}
)

target_compile_definitions(cscope
    PRIVATE HAVE_CONFIG_H=1
)