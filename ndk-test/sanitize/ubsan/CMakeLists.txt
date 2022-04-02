cmake_minimum_required(VERSION 3.5)

# project name
project (ubsan_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../exe)

# Add executable
file(GLOB TESTCASES src/*.cpp)
foreach(srcfile ${TESTCASES})
    get_filename_component(testname ${srcfile} NAME_WE)
    set(CMAKE_CXX_FLAGS "-fsanitize=undefined")

    if (${testname} STREQUAL "add-overflow")
        add_executable(add-overflow-32 ${srcfile})
        target_compile_definitions(add-overflow-32 PRIVATE ADD_I32)
        add_executable(add-overflow-64 ${srcfile})
        target_compile_definitions(add-overflow-64 PRIVATE ADD_I64)
        add_executable(add-overflow-128 ${srcfile})
        target_compile_definitions(add-overflow-128 PRIVATE ADD_I128)
    elseif(${testname} STREQUAL "div-zero")
        add_executable(div-zero-0 ${srcfile})
        target_compile_definitions(div-zero-0 PRIVATE DIVIDEND=0)
        add_executable(div-zero-1U ${srcfile})
        target_compile_definitions(div-zero-1U PRIVATE DIVIDEND=1U)
        add_executable(div-zero-123 ${srcfile})
        target_compile_definitions(div-zero-123 PRIVATE "DIVIDEND=intmax(123)")
        add_executable(div-zero-1-5 ${srcfile})
        target_compile_options(div-zero-1-5 PRIVATE -fsanitize=float-divide-by-zero)
        target_compile_definitions(div-zero-1-5 PRIVATE DIVIDEND=1.5)
    elseif(${testname} STREQUAL "shift")
        set(UBSAN_SHIFT_FLAG "-fno-sanitize-recover=shift")
        set(CMAKE_CXX_FLAGS "")
        # LSH_OVERFLOW
        # CHECK-LSH_OVERFLOW: shift.cpp:[[@LINE+1]]:5: runtime error: left shift of negative value -2147483648
        add_executable(shift-lsh-overflow ${srcfile})
        target_compile_options(shift-lsh-overflow PRIVATE -fsanitize=shift ${UBSAN_SHIFT_FLAG} "-DLSH_OVERFLOW" "-DOP=<<")
        add_executable(shift-lsh-overflow-1 ${srcfile})
        target_compile_options(shift-lsh-overflow-1 PRIVATE -fsanitize=shift-base ${UBSAN_SHIFT_FLAG} "-DLSH_OVERFLOW" "-DOP=<<")
        add_executable(shift-lsh-overflow-2 ${srcfile})
        target_compile_options(shift-lsh-overflow-2 PRIVATE -fsanitize=shift-exponent ${UBSAN_SHIFT_FLAG} "-DLSH_OVERFLOW" "-DOP=<<")
        # TOO_LOW
        # CHECK-TOO_LOW: shift.cpp:[[@LINE+1]]:5: runtime error: shift exponent -3 is negative
        add_executable(shift-too-low ${srcfile})
        target_compile_options(shift-too-low PRIVATE ${UBSAN_SHIFT_FLAG} "-DTOO_LOW" "-DOP=<<")
        # TOO_HIGH
        # CHECK-TOO_HIGH: shift.cpp:[[@LINE+1]]:5: runtime error: shift exponent 32 is too large for 32-bit type 'int'
        add_executable(shift-high ${srcfile})
        target_compile_options(shift-high PRIVATE ${UBSAN_SHIFT_FLAG} "-DTOO_HIGH" "-DOP=>>")
        #end
    elseif(${testname} STREQUAL "uadd-overflow")
        add_executable(uadd-overflow-32 ${srcfile})
        target_compile_definitions(uadd-overflow-32 PRIVATE ADD_I32)
        add_executable(uadd-overflow-64 ${srcfile})
        target_compile_definitions(add-overflow-64 PRIVATE ADD_I64)
        add_executable(uadd-overflow-128 ${srcfile})
        target_compile_definitions(uadd-overflow-128 PRIVATE ADD_I128)
    elseif(${testname} STREQUAL "cast-overflow")
        # ld.lld: error: undefined symbol: __sync_val_compare_and_swap_1
        continue()
    elseif(${testname} STREQUAL "unreachable")
        # undefined symbol: returns_unexpectedly
        continue()
    else()
        add_executable(${testname} ${srcfile})
    endif()  
endforeach()

file(GLOB CASES_C src/*.c)
foreach(src_c ${CASES_C})
    get_filename_component(name_c ${src_c} NAME_WE)
    set(CMAKE_CXX_FLAGS "-fsanitize=undefined")
    if(${name_c} STREQUAL "vla")
        # did not work as expected
        add_executable(vla ${src_c})
        target_compile_options(shift-lsh-overflow-1 PRIVATE -fsanitize=vla-bound -O3)
    endif()
endforeach()