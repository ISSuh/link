project(link_module)

message("Build Link net")

set(LINK_NET_SRC
  ${LINK_NET_PATH}/http/url.cc
)

add_library(${LINK_NET_NAME}
 OBJECT
  ${LINK_NET_SRC}
)

target_link_libraries(${LINK_NET_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  dl
)

add_dependencies(${LINK_NET_NAME}
  ${LINK_BASE_NAME}
)
