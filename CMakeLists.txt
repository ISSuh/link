cmake_minimum_required(VERSION 3.7)
project(link)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM="make -j 4")
# set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
add_compile_options(-g -Wall -Wextra -c)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(LINK_LIBRARY_NAME ${PROJECT_NAME})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_LINK_INCLUDE_DIR ${PROJECT_DIR}/src/link)
set(PROJECT_LINK_SRC_DIR ${PROJECT_DIR}/src/link)

set(BUILD_DIR ${PROJECT_DIR}/build)
set(BUILD_DIR_LINK ${BUILD_DIR}/link)
set(BUILD_DIR_EXAMPLES ${BUILD_DIR}/example)
set(BUILD_DIR_TESTS ${BUILD_DIR}/tests)

set(LINK_BASE_NAME ${LINK_LIBRARY_NAME}_base)
set(LINK_BASE_PATH ${PROJECT_LINK_SRC_DIR}/base)

set(LINK_MODULE_NAME ${LINK_LIBRARY_NAME}_module)
set(LINK_MODULE_PATH ${PROJECT_LINK_SRC_DIR}/module)

set(LINK_NODE_NAME ${LINK_LIBRARY_NAME}_node)
set(LINK_NODE_PATH ${PROJECT_LINK_SRC_DIR}/node)


#-------------------------------------------------------------------------------
# Build Link
#-------------------------------------------------------------------------------
include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(${LINK_BASE_PATH})
add_subdirectory(${LINK_MODULE_PATH})

add_library(${LINK_LIBRARY_NAME}
 STATIC
  $<TARGET_OBJECTS:link_base>
  $<TARGET_OBJECTS:link_module>
)

target_link_libraries(${LINK_LIBRARY_NAME}
 PUBLIC
  ${LINK_BASE_NAME}
  ${LINK_MODULE_NAME}
)

set_target_properties(${LINK_LIBRARY_NAME}
 PROPERTIES
  LINK_FLAGS "-Wl,-all_load"
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