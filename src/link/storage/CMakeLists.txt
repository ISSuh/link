project(link_storage)

message("Build Link storage")

file(GLOB storage_base
  ${LINK_STORAGE_PATH}/base/*.cc
)

file(GLOB storage_database
  ${LINK_STORAGE_PATH}/database/*.cc
  ${LINK_STORAGE_PATH}/adaptor/*.cc
)

add_library(${LINK_STORAGE_NAME}
 OBJECT
  ${storage_base}
  ${storage_database}
)

target_link_libraries(${LINK_STORAGE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}

  odbc
)

add_dependencies(${LINK_STORAGE_NAME}
  ${LINK_BASE_NAME}
)
