project(link_net_test)

message("Build Link Test")

#-------------------------------------------------------------------------------
# Build unit test
#-------------------------------------------------------------------------------

file(GLOB net_base_unittests
  ${PROJECT_SOURCE_DIR}/base/*.cc
)

file(GLOB net_http_unittests
  ${PROJECT_SOURCE_DIR}/http/*.cc
)

add_executable(${PROJECT_NAME}
  ${net_base_unittests}
  ${net_http_unittests}
)

target_include_directories(${PROJECT_NAME}
 PRIVATE
  ${PROJECT_LINK_INCLUDE_DIR}
  ${GOOGLETEST_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
 PRIVATE
  gtest
  gtest_main
  gmock
  gmock_main
  pthread

  ${LINK_LIBRARY_NAME}
)

add_test(
  NAME
    ${PROJECT_NAME}
  COMMAND
    ${PROJECT_NAME}
  WORKING_DIRECTORY
    ${BUILD_DIR_TESTS}
)
