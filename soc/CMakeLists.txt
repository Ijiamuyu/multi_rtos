get_current_component_dir_and_name(component_dir component_name)

set(SOC_INCLUDE "")
set(SOC_SOURCE "")

add_subdirectory(common)
add_subdirectory(console)
add_subdirectory(cpu/${BUILD_ARCH})

list(APPEND GLOBAL_INCLUDE ${SOC_INCLUDE})
list(APPEND GLOBAL_SOURCE ${SOC_SOURCE})

set(GLOBAL_INCLUDE ${GLOBAL_INCLUDE} PARENT_SCOPE)
set(GLOBAL_SOURCE ${GLOBAL_SOURCE} PARENT_SCOPE)
set(LINKER_FLAGS ${LINKER_FLAGS} PARENT_SCOPE)
