cmake_minimum_required(VERSION 3.5)
project(link)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
option(LINK_DEBUG "Debug Build" ON)
option(LINK_WITH_ASIO "Use ASIO" OFF)
option(LINK_WITH_OPENSSL "Use Openssl" OFF)
option(LINK_WITH_TEST "build test" OFF)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINK_DEBUG)
  set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -fPIC -rdynamic -ldl")
else()
  set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -fPIC -rdynamic -ldl")
endif()

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(LINK_LIBRARY_NAME ${PROJECT_NAME})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_LINK_INCLUDE_DIR ${PROJECT_DIR}/src/link)
set(PROJECT_LINK_SRC_DIR ${PROJECT_DIR}/src/link)

set(BUILD_DIR ${PROJECT_DIR}/build)
set(BUILD_DIR_LINK ${BUILD_DIR}/link)
set(BUILD_DIR_EXAMPLES ${BUILD_DIR_LINK}/example)
set(BUILD_DIR_TESTS ${BUILD_DIR}/tests)

set(LINK_BASE_NAME ${LINK_LIBRARY_NAME}_base)
set(LINK_BASE_PATH ${PROJECT_LINK_SRC_DIR}/base)

set(LINK_MODULE_NAME ${LINK_LIBRARY_NAME}_module)
set(LINK_MODULE_PATH ${PROJECT_LINK_SRC_DIR}/module)

set(LINK_NODE_NAME ${LINK_LIBRARY_NAME}_node)
set(LINK_NODE_PATH ${PROJECT_LINK_SRC_DIR}/node)

set(LINK_IO_NAME ${LINK_LIBRARY_NAME}_io)
set(LINK_IO_PATH ${PROJECT_LINK_SRC_DIR}/io)

set(LINK_NET_NAME ${LINK_LIBRARY_NAME}_net)
set(LINK_NET_PATH ${PROJECT_LINK_SRC_DIR}/net)

set(LINK_HANDLE_NAME ${LINK_LIBRARY_NAME}_handle)
set(LINK_HANDLE_PATH ${PROJECT_LINK_SRC_DIR}/handle)

set(LINK_COMPONENT_NAME ${LINK_LIBRARY_NAME}_component)
set(LINK_COMPONENT_PATH ${PROJECT_LINK_SRC_DIR}/component)

#-------------------------------------------------------------------------------
# Install thidparty
#-------------------------------------------------------------------------------
set(LINK_THIRDPATRY_PATH ${PROJECT_LINK_SRC_DIR}/third_party)

set(LINK_THIRD_PARTY_LIST
  json
  spdlog
  uri
)

if(LINK_WITH_ASIO)
  set(LINK_THIRD_PARTY_LIST
    ${LINK_THIRD_PARTY_LIST}
    asio
  )
endif()

foreach(third_party ${LINK_THIRD_PARTY_LIST})
  message("Fetching and Build ${third_party}")
  include(${PROJECT_DIR}/cmake/${third_party}.cmake)
  
  fetch_thridparty(
    ${LINK_THIRDPATRY_PATH}
    ${PROJECT_DIR}/cmake
    ${BUILD_DIR}/${third_party}
  )
endforeach()

set(LINK_THIRDPARTY_LIBS
  spdlog
  network-uri
)

#-------------------------------------------------------------------------------
# Build Link
#-------------------------------------------------------------------------------
include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(${LINK_BASE_PATH})
add_subdirectory(${LINK_MODULE_PATH})
add_subdirectory(${LINK_NODE_PATH})
add_subdirectory(${LINK_IO_PATH})
add_subdirectory(${LINK_NET_PATH})
add_subdirectory(${LINK_HANDLE_PATH})
add_subdirectory(${LINK_COMPONENT_PATH})

add_library(${LINK_LIBRARY_NAME}
 SHARED
  $<TARGET_OBJECTS:link_base> 
  $<TARGET_OBJECTS:link_module>
  $<TARGET_OBJECTS:link_node>
  $<TARGET_OBJECTS:link_io>
  $<TARGET_OBJECTS:link_net>
  $<TARGET_OBJECTS:link_component>
  $<TARGET_OBJECTS:link_handle>
)

target_link_libraries(${LINK_LIBRARY_NAME}
 PUBLIC
  ${LINK_BASE_NAME}
  ${LINK_MODULE_NAME}
  ${LINK_NODE_NAME}
  ${LINK_IO_NAME}
  ${LINK_NET_NAME}
  ${LINK_COMPONENT_NAME}
  ${LINK_HANDLE_NAME}
)

set_target_properties(${LINK_LIBRARY_NAME}
 PROPERTIES
  # LINK_FLAGS "-Wl,-all_load"
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_LINK}
)

#-------------------------------------------------------------------------------
# Build example
#-------------------------------------------------------------------------------
add_subdirectory(example)

#-------------------------------------------------------------------------------
# Set gTest
#-------------------------------------------------------------------------------
if(LINK_WITH_ASIO)
  set(TEST_DIR ${PROJECT_DIR}/tests)

  include(${PROJECT_DIR}/cmake/googletest.cmake)

  fetch_googletest(
      ${PROJECT_DIR}/test
      ${PROJECT_DIR}/cmake
      ${BUILD_DIR}/googletest
  )

  add_subdirectory(${TEST_DIR})
endif()