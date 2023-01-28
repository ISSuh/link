project(link_storage)

message("Build Link storage")

file(GLOB storage_base
  ${LINK_STORAGE_PATH}/base/*.cc
)

file(GLOB storage_database
  ${LINK_STORAGE_PATH}/database/*.cc
)

file(GLOB storage_database_adaptor
  ${LINK_STORAGE_PATH}/database/adaptor/sqlite3/*.cc
)

add_library(${LINK_STORAGE_NAME}
 OBJECT
  ${storage_base}
  ${storage_database}
  ${storage_database_adaptor}
)

target_link_libraries(${LINK_STORAGE_NAME}
 PRIVATE
  ${LINK_BASE_NAME}

  sqlite3
  odbc
)

add_dependencies(${LINK_STORAGE_NAME}
  ${LINK_BASE_NAME}
)
