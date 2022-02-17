cmake_minimum_required(VERSION 3.5)
project(example_test)

message("link test Example")

#-------------------------------------------------------------------------------
# Example Build
#-------------------------------------------------------------------------------
set(test
  test.cc
)

add_executable(test test.cc)

target_link_libraries(test
 PRIVATE
  ${LINK_LIBRARY_NAME}
)