project(link_net)

message("Build Link net")

set(LINK_NET_SRC
  ${LINK_NET_PATH}/base/net_error.cc
  ${LINK_NET_PATH}/base/address_family.cc
  ${LINK_NET_PATH}/base/sockaddr_storage.cc
  ${LINK_NET_PATH}/base/ip_address.cc
  ${LINK_NET_PATH}/base/ip_endpoint.cc

  ${LINK_NET_PATH}/socket/asio/acceptor.cc
  ${LINK_NET_PATH}/socket/asio/session_manager.cc
  ${LINK_NET_PATH}/socket/asio/session.cc
  ${LINK_NET_PATH}/socket/asio/tcp_server.cc
  ${LINK_NET_PATH}/socket/asio/tcp_client.cc
)

add_library(${LINK_NET_NAME}
 OBJECT
  ${LINK_NET_SRC}
)

target_link_libraries(${LINK_NET_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)

add_dependencies(${LINK_NET_NAME}
  ${LINK_BASE_NAME}
)
