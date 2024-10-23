get_current_component_dir_and_name(component_dir component_name)

set(PORT_RISCV_INCLUDE "")
set(PORT_RISCV_SOURCE "")

list(APPEND PORT_RISCV_INCLUDE "${component_dir}")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/port.c")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/context_gcc.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/cpuport.c")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/interrupt_gcc.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/portASM.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/trap_common.c")

list(APPEND PORTABLE_INCLUDE ${PORT_RISCV_INCLUDE})
list(APPEND PORTABLE_SOURCE ${PORT_RISCV_SOURCE})

set(PORTABLE_INCLUDE ${PORTABLE_INCLUDE} PARENT_SCOPE)
set(PORTABLE_SOURCE ${PORTABLE_SOURCE} PARENT_SCOPE)
