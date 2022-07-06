project(link_test)

message("Build Link Test")

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(GOOGLETEST_INCLUDE_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/include)
set(GOOGLETEST_LIB_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/lib)

#-------------------------------------------------------------------------------
# Build Link
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Build node launcher
#-------------------------------------------------------------------------------

include_directories(
  ${PROJECT_LINK_INCLUDE_DIR}
  ${GOOGLETEST_INCLUDE_DIR}
)

link_directories(
  ${GOOGLETEST_LIB_DIR}
)

add_executable(link_unit_tests
  main.cc
)

target_include_directories(link_unit_tests
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
  ${GOOGLETEST_INCLUDE_DIR}
)

target_link_libraries(link_unit_tests
 PRIVATE
  pthread
  gtest
  gtest_main
  gmock
  gmock_main
)
