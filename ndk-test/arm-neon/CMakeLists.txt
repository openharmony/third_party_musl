CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(arm-neon-test)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)

enable_language(C CXX ASM)

set(BUILD_FLAGS
    -mfloat-abi=hard
    -march=armv7a)

function(NEON_TEST WITH_FP16)

    add_executable(arm-neon-fp16 src/arm-neon-fp16.cpp)
    if(WITH_FP16)
        list(APPEND BUILD_FLAGS -mfpu=neon-fp16)
    else()
        list(APPEND BUILD_FLAGS -mfpu=neon)
    endif()
    target_compile_options(arm-neon-fp16 PUBLIC "${BUILD_FLAGS}")

endfunction()

function(ASM_TEST)
    add_library(asm-test OBJECT src/asm-test.s)
    target_compile_options(asm-test PUBLIC "${BUILD_FLAGS}")
endfunction()



NEON_TEST(ON)
# NEON_TEST(OFF)
ASM_TEST()