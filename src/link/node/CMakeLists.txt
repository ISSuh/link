project(link_node)

message("Build Link node")

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-DDEBUG -DLINKER_FLAGS")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_LINK})
#-------------------------------------------------------------------------------
# Build Link
#-------------------------------------------------------------------------------
FILE(GLOB node_base ${LINK_NODE_PATH}/base/*.cc)

add_library(${LINK_NODE_NAME}
 OBJECT
  ${node_base}
)

add_library(${LINK_NODE_NAME}
 OBJECT
  ${LINK_NODE_SRC}
)

target_link_libraries(${LINK_NODE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  ${LINK_MODULE_NAME}  
)

add_dependencies(${LINK_NODE_NAME}
  ${LINK_BASE_NAME}
  ${LINK_MODULE_NAME}
)

#-------------------------------------------------------------------------------
# Build node launcher
#-------------------------------------------------------------------------------
set(node_launcher
  ${LINK_NODE_PATH}/node_launcher.cc
)

add_executable(node_launcher node_launcher.cc)

target_link_libraries(node_launcher
 PRIVATE
  ${LINK_BASE_NAME}
  ${LINK_MODULE_NAME}
  ${LINK_NODE_NAME}
)
