project(link_base)

message("Build Link base")

FILE(GLOB base ${LINK_BASE_PATH}/*.cc)
FILE(GLOB base_task ${LINK_BASE_PATH}/task/*.cc)
FILE(GLOB base_event ${LINK_BASE_PATH}/event/*.cc)
FILE(GLOB base_event_asio ${LINK_BASE_PATH}/event/*.cc)

add_library(${LINK_BASE_NAME}
 OBJECT
  ${base}
  ${base_task}
  ${base_event}
  ${base_event_asio}
)

target_link_libraries(${LINK_BASE_NAME}
 PUBLIC
  pthread
  ${LINK_THIRDPARTY_LIBS}
)
