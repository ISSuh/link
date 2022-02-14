cmake_minimum_required(VERSION 3.5)
project(link)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM="make -j 4")
add_compile_options(-g -Wall -Wextra -c)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(LINK_LIBRARY_NAME ${PROJECT_NAME})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_LINK_INCLUDE_DIR ${PROJECT_DIR}/src/link)
set(PROJECT_LINK_SRC_DIR ${PROJECT_DIR}/src/link)

set(BUILD_DIR_LINK ${PROJECT_DIR}/build/link)
set(BUILD_DIR_EXAMPLES ${PROJECT_DIR}/build/example)
set(BUILD_DIR_TESTS ${PROJECT_DIR}/build/tests)

set(LINK_BASE_PATH ${PROJECT_LINK_SRC_DIR}/base)

#-------------------------------------------------------------------------------
# Build Link
#-------------------------------------------------------------------------------
include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

include(${LINK_BASE_PATH}/CMakeLists.txt)

add_library(${LINK_LIBRARY_NAME}
 STATIC
  ${LINK_BASE_SRC}
)

target_link_libraries(${LINK_LIBRARY_NAME}
 PUBLIC
  pthread
)

#-------------------------------------------------------------------------------
# Build example
#-------------------------------------------------------------------------------
add_subdirectory(example)
add_subdirectory(example/hello_world)

#-------------------------------------------------------------------------------
# Set gTest
#-------------------------------------------------------------------------------
# include(cmake/googletest.cmake)

# fetch_googletest(
#     ${PROJECT_SOURCE_DIR}/cmake
#     ${PROJECT_BINARY_DIR}/googletest
# )

# enable_testing()

# add_subdirectory(tests)