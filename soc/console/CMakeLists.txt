get_current_component_dir_and_name(component_dir component_name)

set(CONSOLE_INCLUDE "")
set(CONSOLE_SOURCE "")

list(APPEND CONSOLE_INCLUDE "${component_dir}")
list(APPEND CONSOLE_SOURCE  "${component_dir}/console.c")


list(APPEND SOC_INCLUDE ${CONSOLE_INCLUDE})
list(APPEND SOC_SOURCE ${CONSOLE_SOURCE})

set(SOC_INCLUDE ${SOC_INCLUDE} PARENT_SCOPE)
set(SOC_SOURCE ${SOC_SOURCE} PARENT_SCOPE)

