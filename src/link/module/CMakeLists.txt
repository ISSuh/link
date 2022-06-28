project(link_module)

message("Build Link module")

set(LINK_MODULE_SRC
  ${LINK_MODULE_PATH}/base/module.cc
  ${LINK_MODULE_PATH}/base/user_module.cc
  ${LINK_MODULE_PATH}/base/user_module_base.cc
  ${LINK_MODULE_PATH}/base/specification.cc

  ${LINK_MODULE_PATH}/loader/module_loader.cc
  ${LINK_MODULE_PATH}/loader/module_register.cc

  ${LINK_MODULE_PATH}/controller/controller.cc
  ${LINK_MODULE_PATH}/controller/executor.cc
)

add_library(${LINK_MODULE_NAME}
 OBJECT
  ${LINK_MODULE_SRC}
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
