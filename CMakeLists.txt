project(cscope)

cmake_minimum_required(VERSION 3.16)

if(WIN32)
  message("fetching winflexbison programs")
  include(FetchContent)
  FetchContent_Declare(
    winflexbison
    URL      https://github.com/lexxmark/winflexbison/releases/download/v2.5.24/win_flex_bison-2.5.24.zip
    URL_HASH SHA256=39C6086CE211D5415500ACC5ED2D8939861CA1696AEE48909C7F6DAF5122B505
  )
  # calculate hash in powershell:  Get-FileHash -Algorithm SHA256 <file name>
  if(NOT winflexbison_POPULATED)
    FetchContent_Populate(winflexbison)
  endif()
  # add exe to path so that cmake can search
  LIST(APPEND CMAKE_PROGRAM_PATH "${winflexbison_SOURCE_DIR}")
endif()

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

# configure curses lib
if(WIN32)
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
    # choose SDL2 backend
    set(PDC_SDL2_BUILD off CACHE BOOL "" FORCE)
    set(PDC_BUILD_SHARED off CACHE BOOL "" FORCE)
    add_subdirectory(${pdcurses_SOURCE_DIR} ${pdcurses_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    # list(APPEND LIBS vt_pdcursesstatic)
    set(HAS_CURSES 1)
else()
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
endif(WIN32)

include(CheckFunctionExists)
check_function_exists(asprintf HAVE_ASPRINTF)
check_function_exists(lstat HAVE_LSTAT)
# sigsetjmp detection is not working correctly
include(CheckSymbolExists)
check_symbol_exists(sigsetjmp "setjmp.h" HAVE_SIGSETJMP)

# configure dirent lib
if(WIN32)
    message("fetching dirent for win")
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
else()
    include(CheckIncludeFile)
    check_include_file(dirent.h HAVE_DIRENT_H)
    if(NOT HAVE_DIRENT_H)
        message(FATAL_ERROR "dirent.h not found.")
    endif()
endif(WIN32)

# configure regex lib
if(WIN32)
    message("fetching tre for regex")
    include(FetchContent)
    FetchContent_Declare(tre
        GIT_REPOSITORY    https://github.com/youyuanwu/tre.git
        GIT_TAG           ca197f0999f647fa273370e984e4f95a6c9cdd60
    )
    FetchContent_MakeAvailable(tre)
endif(WIN32)

include(CheckIncludeFile)
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

if(WIN32)
    target_include_directories(cscope
        PRIVATE ${dirent_SOURCE_DIR}/include
        PRIVATE ${tre_SOURCE_DIR}/include
        PRIVATE ${tre_SOURCE_DIR}/lib
    )
    target_include_directories(cscope PRIVATE ${pdcurses_SOURCE_DIR})
    message(" debug: ${pdcurses_INCLUDES_DIR}")
    target_link_libraries(cscope PRIVATE pdcurses tre)
endif()

target_link_libraries(cscope
    PRIVATE ${CURSES_LIBRARIES}
)

target_compile_definitions(cscope
    PRIVATE HAVE_CONFIG_H=1
)