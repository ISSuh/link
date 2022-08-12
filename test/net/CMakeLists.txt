project(link_net_test)

message("Build Link Test")

#-------------------------------------------------------------------------------
# Build unit test
#-------------------------------------------------------------------------------

set(NET_BASE_UNITTESTS
  ${PROJECT_SOURCE_DIR}/base/url_unittest.cc
)

# add_executable(${PROJECT_NAME}
#   ${NET_BASE_UNITTESTS}
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