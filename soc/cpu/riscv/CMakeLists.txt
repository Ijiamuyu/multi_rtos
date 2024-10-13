get_current_component_dir_and_name(component_dir component_name)

set(CPU_INCLUDE "")
set(CPU_SOURCE "")

list(APPEND CPU_INCLUDE "${component_dir}")
list(APPEND CPU_SOURCE  "${component_dir}/cpu.c")
list(APPEND CPU_SOURCE  "${component_dir}/isr.c")
list(APPEND CPU_SOURCE  "${component_dir}/start.S")

add_compile_options(
    -march=rv32imac -mabi=ilp32 -mcmodel=medany
    -Wall -fmessage-length=0 -ffunction-sections
    -fdata-sections -fno-builtin-printf
    -Og -ggdb3 -D__riscv_float_abi_soft
    )


set(LINKER_SCRIPT "${component_dir}/fake_rom.lds")
set(LINKER_FLAGS "-march=rv32imac -mabi=ilp32 -T ${LINKER_SCRIPT} -Wl,--Map=${PROJECT_BINARY_DIR}/${TARGET_NAME}.map -nostartfiles -Xlinker --gc-sections -Xlinker --defsym=__stack_size=300 -D__riscv_float_abi_soft")


list(APPEND GLOBAL_INCLUDE ${CPU_INCLUDE})
list(APPEND GLOBAL_SOURCE ${CPU_SOURCE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)
set(LINKER_FLAGS ${LINKER_FLAGS} PARENT_SCOPE)
