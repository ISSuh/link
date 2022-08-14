cmake_minimum_required(VERSION 3.5)
project(example_http_server_client)

message("link module HTTP Example")

set(SERVER_EXAMPLE "httpserver_example")
set(CLIENT_EXAMPLE "httpclient_example")

#-------------------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC -rdynamic -ldl")

#-------------------------------------------------------------------------------
# Linking option about debug, release
#-------------------------------------------------------------------------------
# SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -DC_FLAGS")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-DDEBUG -DLINKER_FLAGS")

#-------------------------------------------------------------------------------
# Example Build
#-------------------------------------------------------------------------------
include_directories(
  ${PROJECT_LINK_INCLUDE_DIR}
)

link_directories(
  ${BUILD_DIR}
)

# Server Example
add_library(${SERVER_EXAMPLE}
 MODULE
  server/server_module.cc
  server/example_http_server.cc
)

target_include_directories(${SERVER_EXAMPLE}
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
)

target_link_libraries(${SERVER_EXAMPLE}
 PRIVATE
  ${LINK_LIBRARY_NAME}
)

set_target_properties(${SERVER_EXAMPLE}
 PROPERTIES
  LINKER_LANGUAGE CXX 
  # LINK_FLAGS "-Wl,-all_load"
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/http
)

# Server Example
add_library(${CLIENT_EXAMPLE}
 MODULE
  client/client_module.cc
  client/example_http_client.cc
)

target_include_directories(${CLIENT_EXAMPLE}
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
)

target_link_libraries(${CLIENT_EXAMPLE}
 PRIVATE
  ${LINK_LIBRARY_NAME}
)

set_target_properties(${CLIENT_EXAMPLE}
 PROPERTIES
  LINKER_LANGUAGE CXX 
  # LINK_FLAGS "-Wl,-all_load"
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/http
)