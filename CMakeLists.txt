set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ${BUILD_ARCH})
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(MULTI_RTOS C ASM CXX)

function(get_current_component_dir_and_name _ret_component_dir _ret_component_name)
    get_filename_component(_component_dir ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
    get_filename_component(_component_name ${_component_dir} NAME)
    set(${_ret_component_dir} ${_component_dir} PARENT_SCOPE)
    set(${_ret_component_name} ${_component_name} PARENT_SCOPE)
endfunction()

if(BUILD_ARCH STREQUAL arm)
    set(CROSS_COMPILE "")
elseif(BUILD_ARCH STREQUAL riscv)
    set(CROSS_COMPILE "riscv64-unknown-elf-")
else()
    message(FATAL_ERROR "Invalid ARCH to build!")
endif()

set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_LINKER ${CROSS_COMPILE}ld)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)
set(CMAKE_SIZE ${CROSS_COMPILE}size)
set(CMAKE_GDB ${CROSS_COMPILE}gdb)

set(GLOBAL_INCLUDE "")
set(GLOBAL_SOURCE "")

add_subdirectory(app)
add_subdirectory(driver)
add_subdirectory(kernel/${RTOS})
add_subdirectory(port/${RTOS})
add_subdirectory(soc)

if(BUILD_ARCH STREQUAL riscv)
    add_compile_options(
        -march=rv32imac -mabi=ilp32 -mcmodel=medany
        -Wall -fmessage-length=0 -ffunction-sections
        -fdata-sections -fno-builtin-printf
        -Og -ggdb3 -D__riscv_float_abi_soft
    )
elseif(BUILD_ARCH STREQUAL arm)
    message(FATAL_ERROR "Invalid board to build!")
else()
    message(FATAL_ERROR "Invalid board to build!")
endif()

set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

include_directories(${GLOBAL_INCLUDE})

add_executable(${TARGET_NAME}.out ${GLOBAL_SOURCE})
