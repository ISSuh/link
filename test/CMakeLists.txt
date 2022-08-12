project(link_test)

message("Build Link Test")

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(GOOGLETEST_INCLUDE_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/include)
set(GOOGLETEST_LIB_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/lib)

include_directories(
  ${PROJECT_LINK_INCLUDE_DIR}
  ${GOOGLETEST_INCLUDE_DIR}
)

link_directories(
  ${GOOGLETEST_LIB_DIR}
  ${BUILD_DIR}
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------
# add_subdirectory(io)
# add_subdirectory(net)

include(${TEST_DIR}/io/CmakeLists.txt)

add_executable(${PROJECT_NAME}
  ${IO_BASE_UNITTESTS}
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
    ${PROJECT_NAME}_Tests
)
