cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(@THIRD_PARTY_NAME@-fetch NONE)

include(ExternalProject)

ExternalProject_Add(@THIRD_PARTY_NAME@
  GIT_REPOSITORY  https://github.com/chriskohlhoff/asio
  GIT_TAG         asio-1-22-0

  SOURCE_DIR    @THIRD_PARTY_DOWNLOAD_SRC_DIR@
  BINARY_DIR    @THIRD_PARTY_DOWNLOAD_BUILD_DIR@

  CONFIGURE_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@/asio &&
    ./autogen.sh &&
    ./configure --prefix=@THIRD_PARTY_DOWNLOAD_INSTALL_DIR@

  BUILD_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@/asio &&
    make -j${CONCURRENCY}

  INSTALL_COMMAND
    cd @THIRD_PARTY_DOWNLOAD_SRC_DIR@/asio &&
    make install
)
