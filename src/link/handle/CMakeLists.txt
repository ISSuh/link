project(link_handle)

message("Build Link handle")

FILE(GLOB handle ${LINK_HANDLE_PATH}/*.cc)

add_library(${LINK_HANDLE_NAME}
 OBJECT
  ${handle}
)

target_link_libraries(${LINK_HANDLE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
)
