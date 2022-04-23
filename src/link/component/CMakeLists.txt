project(link_component)

message("Build Link component")

set(LINK_COMPONENT_SRC

)

add_library(${LINK_COMPONENT_NAME}
 OBJECT
  ${LINK_COMPONENT_SRC}
)

target_link_libraries(${LINK_COMPONENT_NAME}
 PRIVATE
  ${LINK_BASE_NAME}
  ${LINK_NET_NAME}
)

add_dependencies(${LINK_COMPONENT_NAME}
  ${LINK_BASE_NAME}
  ${LINK_NET_NAME}
)
