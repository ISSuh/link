cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(glog-download NONE)

include(ExternalProject)

ExternalProject_Add(glog
  GIT_REPOSITORY
    https://github.com/google/glog.git
  GIT_TAG
    v0.6.0

  SOURCE_DIR  "@GLOG_DOWNLOAD_ROOT@/glog-src"
  BINARY_DIR  "@GLOG_DOWNLOAD_ROOT@/glog-build"
  INSTALL_DIR "@GLOG_DOWNLOAD_ROOT@/glog-install"

  PRIFIX  "${CMAKE_CURRENT_BINARY_DIR}"
  CMAKE_ARGS  "-DCMAKE_INSTALL_PREFIX = ${CMAKE_CURRENT_BINARY_DIR}/glog-install"

  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
