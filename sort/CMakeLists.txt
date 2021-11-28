
file(GLOB SOURCES
    include/*.h
    src/*.c
)

add_library(cscope_sort STATIC 
    ${SOURCES}
)

target_include_directories(
    cscope_sort
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tests:

# message("fetching unity")
# include(FetchContent)
# FetchContent_Declare(unity
#     GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
#     GIT_TAG b19370cc2b6412f885c57df95f43e4502cdc0aed
# )
# FetchContent_MakeAvailable(unity)
# import google test
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES
    test/*_test.cpp
)

# strip file extension
#message("sources are  " ${SOURCES})
foreach(test_file ${SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE gtest_main cscope_sort)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()