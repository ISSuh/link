project(link_component)

message("Build Link component")

file(GLOB component_base
  ${LINK_COMPONENT_PATH}/base/*.cc
)

file(GLOB socket_component
  ${LINK_COMPONENT_PATH}/ipc/socket/*.cc
)

file(GLOB http_component
  ${LINK_COMPONENT_PATH}/http/*.cc
  ${LINK_COMPONENT_PATH}/http/server/*.cc
)

add_library(${LINK_COMPONENT_NAME}
 OBJECT
  ${component_base}
  ${socket_component}
  ${http_component}
)

target_link_libraries(${LINK_COMPONENT_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  ${LINK_NET_NAME}
)

add_dependencies(${LINK_COMPONENT_NAME}
  ${LINK_BASE_NAME}
  ${LINK_NET_NAME}
)
