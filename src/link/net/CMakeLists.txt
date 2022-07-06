project(link_module)

message("Build Link net")

FILE(GLOB net_base ${LINK_NET_PATH}/base/*.cc)
FILE(GLOB net_http ${LINK_NET_PATH}/http/*.cc)

add_library(${LINK_NET_NAME}
 OBJECT
  ${net_base}
  ${net_http}
)

target_link_libraries(${LINK_NET_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  dl
)

add_dependencies(${LINK_NET_NAME}
  ${LINK_BASE_NAME}
)
