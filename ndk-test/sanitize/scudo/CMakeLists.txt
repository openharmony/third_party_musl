cmake_minimum_required(VERSION 3.5)

# project name
project (scudo_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../exe)

# Add executable
file(GLOB TESTCASES src/*.cpp )
foreach(srcfile ${TESTCASES})
    get_filename_component(testname ${srcfile} NAME_WE)
    add_executable(${testname} ${srcfile})
endforeach()

file(GLOB TESTCASES_C src/*.c )
foreach(srcfile_c ${TESTCASES_C})
    get_filename_component(testname_c ${srcfile_c} NAME_WE)
    add_executable(${testname_c} ${srcfile_c})
    # include_directories("${PROJECT_SOURCE_DIR}/include")
endforeach()