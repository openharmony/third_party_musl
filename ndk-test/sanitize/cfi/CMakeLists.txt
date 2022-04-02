cmake_minimum_required(VERSION 3.5)

# project name
project (asan_test)

set(EXECUTABLE_OUTPUT_PATH build/out)

# Add executable
file(GLOB TESTCASES src/*.cpp )
foreach(srcfile IN LISTS ${TESTCASES})
    get_filename_component(testname ${srcfile} NAME_WE)
    add_executable( ${testname} ${srcfile})
endforeach()
