get_current_component_dir_and_name(component_dir component_name)

set(DRIVER_INCLUDE "")
set(DRIVER_SOURCE "")

list(APPEND DRIVER_INCLUDE "${component_dir}")
list(APPEND DRIVER_SOURCE  "${component_dir}/ns16550.c")

list(APPEND GLOBAL_INCLUDE ${DRIVER_INCLUDE})
list(APPEND GLOBAL_SOURCE ${DRIVER_SOURCE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)

