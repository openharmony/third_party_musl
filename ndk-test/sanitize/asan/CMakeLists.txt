cmake_minimum_required(VERSION 3.5)

# project name
project (asan_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../exe) 

# Add executable
file(GLOB TESTCASES src/*.cpp)
foreach(srcfile ${TESTCASES})
    get_filename_component(testname ${srcfile} NAME_WE)
    add_executable( ${testname} ${srcfile})
endforeach(add_subdirectory(src/init-order))

add_subdirectory(src/init-order)
