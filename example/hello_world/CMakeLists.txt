CMAKE_MINIMUM_REQUIRED (VERSION 3.2)
project(hello_world)

message("link module hello_world Example")

#-------------------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------------------
SET (CMAKE_CXX_STANDARD 11)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-g -Wall -fPIC -rdynamic -ldl -C")

#-------------------------------------------------------------------------------
# Linking option about debug, release
#-------------------------------------------------------------------------------
# SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -DC_FLAGS")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "-DDEBUG -DLINKER_FLAGS")

#-------------------------------------------------------------------------------
# Example Build
#-------------------------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_LIBRARY})

include_directories(
  ${PROJECT_LINK_INCLUDE_DIR}
)

link_directories(
  ${BUILD_DIR_LIBRARY}
)

SET(EXAMPLE_SOURCE
  hello_world_module.cpp
)

add_library(hello_world SHARED ${EXAMPLE_SOURCE})
target_include_directories(hello_world PUBLIC ${SMT_INCLUDES})
target_link_libraries(hello_world PUBLIC smt)

set_target_properties(hello_world PROPERTIES LINKER_LANGUAGE CXX)