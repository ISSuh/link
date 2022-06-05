project(link_io)

message("Build Link io")

set(LINK_IO_SRC
  ${LINK_IO_PATH}/base/net_error.cc
  ${LINK_IO_PATH}/base/address_family.cc
  ${LINK_IO_PATH}/base/sockaddr_storage.cc
  ${LINK_IO_PATH}/base/ip_address.cc
  ${LINK_IO_PATH}/base/ip_endpoint.cc

  ${LINK_IO_PATH}/socket/asio/tcp_session.cc
  ${LINK_IO_PATH}/socket/asio/tcp_server.cc
  ${LINK_IO_PATH}/socket/asio/tcp_client.cc
  ${LINK_IO_PATH}/socket/asio/acceptor.cc
  ${LINK_IO_PATH}/socket/asio/connector.cc

)

add_library(${LINK_IO_NAME}
 OBJECT
  ${LINK_IO_SRC}
)

target_link_libraries(${LINK_IO_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)

add_dependencies(${LINK_IO_NAME}
  ${LINK_BASE_NAME}
)
