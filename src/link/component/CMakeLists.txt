project(link_component)

message("Build Link component")

FILE(GLOB component_base base/*.cc)
FILE(GLOB socket_component ipc/socket/*.cc)
FILE(GLOB http_component http/*.cc)

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
