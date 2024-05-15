set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(EZOS C ASM CXX)

function(get_current_component_dir_and_name _ret_component_dir _ret_component_name)
    get_filename_component(_component_dir ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
    get_filename_component(_component_name ${_component_dir} NAME)
    set(${_ret_component_dir} ${_component_dir} PARENT_SCOPE)
    set(${_ret_component_name} ${_component_name} PARENT_SCOPE)
endfunction()

set(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
set(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
set(CMAKE_ASM_COMPILER riscv-nuclei-elf-gcc)
set(CMAKE_LINKER riscv-nuclei-elf-ld)
set(CMAKE_OBJCOPY riscv-nuclei-elf-objcopy)
set(CMAKE_OBJDUMP riscv-nuclei-elf-objdump)
set(CMAKE_SIZE riscv-nuclei-elf-size)
set(CMAKE_GDB riscv-nuclei-elf-gdb)

set(GLOBAL_INCLUDE "")
set(GLOBAL_SOURCE "")
set(GLOBAL_ASM_SOURCE "")
add_compile_options(-march=rv32imacp -mabi=ilp32 -mcmodel=medany)
add_compile_options(-Wall)
add_compile_options(-fmessage-length=0 -ffunction-sections -fdata-sections -fno-builtin-printf)
add_compile_options(-Og -ggdb3 -D__riscv_float_abi_soft)
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/fake_rom.lds")
set(LINK_PROJECT_FLAGS "-march=rv32imacp -mabi=ilp32 -T ${LINKER_SCRIPT} -Wl,--Map=${PROJECT_BINARY_DIR}/${TARGET_NAME}.map -nostartfiles -Xlinker --gc-sections -Xlinker --defsym=__stack_size=300 -D__riscv_float_abi_soft")


add_subdirectory(app)
add_subdirectory(driver)
add_subdirectory(kernel)
add_subdirectory(portable)
add_subdirectory(include)

set(CMAKE_EXE_LINKER_FLAGS ${LINK_PROJECT_FLAGS})

include_directories(${GLOBAL_INCLUDE})

add_executable(${TARGET_NAME}.out ${GLOBAL_ASM_SOURCE} ${GLOBAL_SOURCE})
