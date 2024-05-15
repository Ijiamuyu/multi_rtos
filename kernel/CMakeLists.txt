get_current_component_dir_and_name(component_dir component_name)

set(KERNEL_INCLUDE "")
set(KERNEL_SOURCE "")

list(APPEND KERNEL_INCLUDE "${component_dir}/include")
list(APPEND KERNEL_SOURCE "${component_dir}/source/ez_core.c")
list(APPEND KERNEL_SOURCE "${component_dir}/source/ez_task.c")
list(APPEND KERNEL_SOURCE "${component_dir}/source/ez_var.c")

list(APPEND GLOBAL_INCLUDE ${KERNEL_INCLUDE})
list(APPEND GLOBAL_SOURCE ${KERNEL_SOURCE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)

