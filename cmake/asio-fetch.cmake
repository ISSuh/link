cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(asio-fetch NONE)

include(ExternalProject)

ExternalProject_Add(asio
  GIT_REPOSITORY  https://github.com/chriskohlhoff/asio
  GIT_TAG         asio-1-22-0

  SOURCE_DIR    @ASIO_DOWNLOAD_ROOT@/asio-src
  BINARY_DIR    @ASIO_DOWNLOAD_ROOT@/asio-build

  CONFIGURE_COMMAND
    cd @ASIO_DOWNLOAD_ROOT@/asio-src/asio &&
    ./autogen.sh &&
    ./configure --prefix=@ASIO_DOWNLOAD_ROOT@/asio-install

  BUILD_COMMAND
    cd @ASIO_DOWNLOAD_ROOT@/asio-src/asio &&
    make -j${CONCURRENCY}

  INSTALL_COMMAND
    cd @ASIO_DOWNLOAD_ROOT@/asio-src/asio &&
    make install
)
