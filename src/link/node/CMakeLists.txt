project(link_node)

message("Build Link node")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_LINK})

set(node_launcher
  ${LINK_NODE_PATH}/node_launcher.cc
)

add_executable(node_launcher node_launcher.cc)

target_link_libraries(node_launcher
 PRIVATE
  ${LINK_BASE_NAME}  
  ${LINK_MODULE_NAME}
)
