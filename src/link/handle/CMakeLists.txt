project(link_handle)

message("Build Link handle")

set(LINK_HANDLE_SRC
  ${LINK_HANDLE_PATH}/link_handle.cc
)

add_library(${LINK_HANDLE_NAME}
 OBJECT
  ${LINK_HANDLE_SRC}
)

target_link_libraries(${LINK_HANDLE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)

add_dependencies(${LINK_NET_NAME}
  ${LINK_BASE_NAME}
)