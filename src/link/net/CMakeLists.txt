project(link_module)

message("Build Link net")

set(LINK_NET_SRC
  ${LINK_NET_PATH}/http/version.cc
  ${LINK_NET_PATH}/http/method.cc
  ${LINK_NET_PATH}/http/url.cc
  ${LINK_NET_PATH}/http/header.cc
  ${LINK_NET_PATH}/http/request.cc
  ${LINK_NET_PATH}/http/parser.cc
  ${LINK_NET_PATH}/http/request_parser.cc
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
