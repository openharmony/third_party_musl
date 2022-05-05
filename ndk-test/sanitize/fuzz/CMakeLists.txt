cmake_minimum_required(VERSION 3.5)

# project name
project (fuzz_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../exe)
set(CMAKE_EXE_LINKER_FLAGS -fsanitize=fuzzer)

# Add executable
file(GLOB TESTCASES src/*.cpp)
foreach(srcfile ${TESTCASES})
    get_filename_component(testname ${srcfile} NAME_WE)
    add_executable(${testname} ${srcfile})
    # target_compile_options(${testname} PRIVATE -fsanitize=fuzzer)
endforeach()