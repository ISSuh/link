project(Example)

message("Link app Examples")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES} )

set(EXAMPLE_SRC
  example
)

foreach(EXAMPLE_NAME ${EXAMPLE_SRC})
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.cc)

  target_include_directories(${EXAMPLE_NAME}
   PUBLIC
    ${PROJECT_LINK_INCLUDE_DIR}
  )

  target_link_libraries(${EXAMPLE_NAME}
   PUBLIC
    ${LINK_LIBRARY_NAME}
  )

  set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "example")
endforeach()
