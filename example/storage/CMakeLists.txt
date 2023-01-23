cmake_minimum_required(VERSION 3.5)
project(example_storage)

message("link module storage Example")

#-------------------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC -ldl")

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

add_library(storage_sample
 MODULE
  storage_module.cc
)

target_include_directories(storage_sample
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
)

target_link_libraries(storage_sample
 PRIVATE
  ${LINK_LIBRARY_NAME}
)

set_target_properties(storage_sample
 PROPERTIES
  LINKER_LANGUAGE CXX 
  LINK_FLAGS "-Wl,-force_load"
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/storage
)