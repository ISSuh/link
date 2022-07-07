project(link_io)

message("Build Link io")

FILE(GLOB io_base ${LINK_IO_PATH}/base/*.cc)

if(LINK_WITH_ASIO)
  FILE(GLOB io_socket_platform ${LINK_IO_PATH}/socket/asio/*.cc)
else()
  FILE(GLOB io_socket_platform ${LINK_IO_PATH}/socket/*.cc)
endif()

add_library(${LINK_IO_NAME}
 OBJECT
  ${io_base}
  ${io_socket_platform}
)

target_link_libraries(${LINK_IO_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)

add_dependencies(${LINK_IO_NAME}
  ${LINK_BASE_NAME}
)
