project(link_io_test)

message("Build Link IO Test")

#-------------------------------------------------------------------------------
# Build unit test
#-------------------------------------------------------------------------------

set(IO_BASE_UNITTESTS
  ${CMAKE_CURRENT_SOURCE_DIR}/io/base/io_address_unittest.cc
)

# add_executable(${PROJECT_NAME}
#   ${IO_BASE_UNITTESTS}
# )

# target_include_directories(${PROJECT_NAME}
#  PRIVATE
#   ${PROJECT_LINK_INCLUDE_DIR}
#   ${GOOGLETEST_INCLUDE_DIR}
# )

# target_link_libraries(${PROJECT_NAME}
#  PRIVATE
#   gtest
#   gtest_main
#   gmock
#   gmock_main
#   pthread

#   ${LINK_LIBRARY_NAME}
# )

# add_test(
#   NAME
#     ${PROJECT_NAME}
#   COMMAND
#     ${PROJECT_NAME}_Tests
# )