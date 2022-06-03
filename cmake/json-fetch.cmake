cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(json-fetch NONE)

include(ExternalProject)

ExternalProject_Add(json
  URL https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.tar.gz

  SOURCE_DIR    @JSON_DOWNLOAD_ROOT@/json-src
  BINARY_DIR    @JSON_DOWNLOAD_ROOT@/json-build

  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=@JSON_DOWNLOAD_ROOT@/json-install
    -DJSON_BuildTests=OFF
)
