cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(@THIRD_PARTY_NAME@-fetch NONE)

include(ExternalProject)

ExternalProject_Add(@THIRD_PARTY_NAME@
  GIT_REPOSITORY  https://github.com/h2o/picohttpparser
  GIT_TAG         master

  SOURCE_DIR    @THIRD_PARTY_DOWNLOAD_SRC_DIR@
  BINARY_DIR    @THIRD_PARTY_DOWNLOAD_BUILD_DIR@

  CONFIGURE_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@

  BUILD_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@ &&
    g++ -c -o picohttpparser.o -Wall -fPIC picohttpparser.c &&
    ar rcs libpicohttpparser.a picohttpparser.o

  INSTALL_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@ &&
    mkdir -p @THIRD_PARTY_DOWNLOAD_INSTALL_DIR@/include/picohttpparser &&
    mkdir -p @THIRD_PARTY_DOWNLOAD_INSTALL_DIR@/lib &&
    scp picohttpparser.h @THIRD_PARTY_DOWNLOAD_INSTALL_DIR@/include/picohttpparser &&
    scp libpicohttpparser.a @THIRD_PARTY_DOWNLOAD_INSTALL_DIR@/lib
)
