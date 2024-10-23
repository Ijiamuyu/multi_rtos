get_current_component_dir_and_name(component_dir component_name)

set(KERNEL_INCLUDE "")
set(KERNEL_SOURCE "")

list(APPEND KERNEL_INCLUDE "${component_dir}/inc")
list(APPEND KERNEL_SOURCE "${component_dir}/src/clock.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/idle.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/ipc.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/irq.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/kservice.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/mem.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/memheap.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/mempool.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/object.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/scheduler.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/slab.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/thread.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/timer.c")
list(APPEND KERNEL_SOURCE "${component_dir}/src/rt_scheduler.c")

list(APPEND GLOBAL_INCLUDE ${KERNEL_INCLUDE})
list(APPEND GLOBAL_SOURCE ${KERNEL_SOURCE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)

