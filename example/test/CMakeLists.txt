cmake_minimum_required(VERSION 3.5)
project(example_test)

message("link test Example")

set(CMAKE_CXX_FLAGS "-g -Wall -fPIC -rdynamic -ldl")
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

add_executable(test test.cc)

target_link_libraries(test
 PRIVATE
  ${LINK_LIBRARY_NAME}
)
set_target_properties(test
 PROPERTIES
  LINKER_LANGUAGE CXX 
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/test
)
