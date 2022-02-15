project(link_node)

message("Build Link base")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_LINK})

set(LINK_NODE_NAME link_node)

set(LINK_NODE_SRC
  ${LINK_MODULE_PATH}/base/module.cc
)

add_library(${LINK_BASE_NAME}
 STATIC
  ${LINK_BASE_SRC}
)

target_link_libraries(${LINK_BASE_NAME}
 PUBLIC
  pthread
)
