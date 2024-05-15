get_current_component_dir_and_name(component_dir component_name)

set(PORTABLE_SOURCE "")
set(PORTABLE_ASMSOURCE "")

list(APPEND PORTABLE_SOURCE "${component_dir}/ez_cpu.c")
list(APPEND PORTABLE_SOURCE "${component_dir}/isr.c")
list(APPEND PORTABLE_SOURCE "${component_dir}/riscv-virt.c")
list(APPEND PORTABLE_ASM_SOURCE "${component_dir}/start.S")
list(APPEND PORTABLE_ASM_SOURCE "${component_dir}/portASM.S")

list(APPEND GLOBAL_SOURCE ${PORTABLE_SOURCE})
list(APPEND GLOBAL_ASM_SOURCE ${PORTABLE_ASM_SOURCE})

set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)
set(GLOBAL_ASM_SOURCE ${GLOBAL_ASM_SOURCE} PARENT_SCOPE)

