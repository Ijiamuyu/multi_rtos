get_current_component_dir_and_name(component_dir component_name)

set(INCLUDE "")

list(APPEND INCLUDE "${component_dir}")

list(APPEND GLOBAL_INCLUDE ${INCLUDE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)

