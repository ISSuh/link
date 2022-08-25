project(link_base_test)

message("Build Link base Test")

#-------------------------------------------------------------------------------
# Build unit test
#-------------------------------------------------------------------------------

file(GLOB base_unittests
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

add_executable(${PROJECT_NAME}
  ${base_unittests}
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
