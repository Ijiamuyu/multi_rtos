get_current_component_dir_and_name(component_dir component_name)

set(COMMON_INCLUDE "")
set(COMMON_SOURCE "")

list(APPEND COMMON_INCLUDE "${component_dir}")
list(APPEND COMMON_SOURCE  "${component_dir}/common.c")


list(APPEND SOC_INCLUDE ${COMMON_INCLUDE})
list(APPEND SOC_SOURCE ${COMMON_SOURCE})

set(SOC_INCLUDE ${SOC_INCLUDE} PARENT_SCOPE)
set(SOC_SOURCE ${SOC_SOURCE} PARENT_SCOPE)

