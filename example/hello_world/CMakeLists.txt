cmake_minimum_required(VERSION 3.5)
project(example_hello_world)

message("link module hello_world Example")

#-------------------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
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

add_library(hello_world
 MODULE
  hello_world_module.cc
)

target_include_directories(hello_world
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
)

target_link_libraries(hello_world
 PRIVATE
  ${LINK_LIBRARY_NAME}
)

set_target_properties(hello_world
 PROPERTIES
  LINKER_LANGUAGE CXX 
  LINK_FLAGS "-Wl,-force_load"
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/hello_world
)