project(link_module)

message("Build Link module")


FILE(GLOB module_base ${LINK_MODULE_PATH}/base/*.cc)
FILE(GLOB module_loader ${LINK_MODULE_PATH}/loader/*.cc)
FILE(GLOB module_controller ${LINK_MODULE_PATH}/controller/*.cc)

add_library(${LINK_MODULE_NAME}
 OBJECT
  ${module_base}
  ${module_loader}
  ${module_controller}
)

target_link_libraries(${LINK_MODULE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  dl
)

set_target_properties(${LINK_MODULE_NAME}
 PROPERTIES
  LINK_FLAGS "-Wl,-all_load"
)

add_dependencies(${LINK_MODULE_NAME}
  ${LINK_BASE_NAME}
)
