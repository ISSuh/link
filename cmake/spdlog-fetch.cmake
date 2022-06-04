cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(@THIRD_PARTY_NAME@-fetch NONE)

include(ExternalProject)

ExternalProject_Add(@THIRD_PARTY_NAME@
  GIT_REPOSITORY  https://github.com/gabime/spdlog.git
  GIT_TAG         v1.10.0

  SOURCE_DIR    @THIRD_PARTY_DOWNLOAD_SRC_DIR@
  BINARY_DIR    @THIRD_PARTY_DOWNLOAD_BUILD_DIR@

  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=@THIRD_PARTY_DOWNLOAD_INSTALL_DIR@

)
