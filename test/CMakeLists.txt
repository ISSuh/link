project(link_test)

message("Build Link Test")

include(CTest) 

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
enable_testing()

add_subdirectory(io)
add_subdirectory(net)
