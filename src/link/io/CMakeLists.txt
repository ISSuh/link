project(link_io)

message("Build Link io")

FILE(GLOB io_base ${LINK_IO_PATH}/base/*.cc)
FILE(GLOB io_socket_asio ${LINK_IO_PATH}/socket/asio/*.cc)

add_library(${LINK_IO_NAME}
 OBJECT
  ${io_base}
  ${io_socket_asio}
)

target_link_libraries(${LINK_IO_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)

add_dependencies(${LINK_IO_NAME}
  ${LINK_BASE_NAME}
)
