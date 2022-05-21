project(link_component)

message("Build Link component")

set(LINK_COMPONENT_SRC
  ${LINK_COMPONENT_PATH}/ipc/socket/tcp_server_component.cc
  ${LINK_COMPONENT_PATH}/ipc/socket/tcp_client_component.cc

  ${LINK_COMPONENT_PATH}/rpc/rpc_server_component.cc
  ${LINK_COMPONENT_PATH}/rpc/rpc_client_component.cc
)

add_library(${LINK_COMPONENT_NAME}
 OBJECT
  ${LINK_COMPONENT_SRC}
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
