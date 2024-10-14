get_current_component_dir_and_name(component_dir component_name)

set(PORT_RISCV_INCLUDE "")
set(PORT_RISCV_SOURCE "")

list(APPEND PORT_RISCV_INCLUDE "${component_dir}")

list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_context_save.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_schedule.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_system_return.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_initialize_low_level.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_context_restore.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_interrupt_control.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_thread_stack_build.S")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/tx_timer_interrupt.c")
list(APPEND PORT_RISCV_SOURCE "${component_dir}/port.c")

list(APPEND PORTABLE_INCLUDE ${PORT_RISCV_INCLUDE})
list(APPEND PORTABLE_SOURCE ${PORT_RISCV_SOURCE})

set(PORTABLE_INCLUDE ${PORTABLE_INCLUDE} PARENT_SCOPE)
set(PORTABLE_SOURCE ${PORTABLE_SOURCE} PARENT_SCOPE)
