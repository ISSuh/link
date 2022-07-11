project(link_base)

message("Build Link base")

FILE(GLOB base ${LINK_BASE_PATH}/*.cc)
FILE(GLOB base_task ${LINK_BASE_PATH}/task/*.cc)
FILE(GLOB base_event ${LINK_BASE_PATH}/event/*.cc)

if(LINK_WITH_ASIO)
  FILE(GLOB base_event_platform ${LINK_BASE_PATH}/event/platform/asio/*.cc)
else()
  FILE(GLOB base_event_platform ${LINK_BASE_PATH}/event/platform/*.cc)
endif()

add_library(${LINK_BASE_NAME}
 OBJECT
  ${base}
  ${base_task}
  ${base_event}
  ${base_event_platform}
)

target_link_libraries(${LINK_BASE_NAME}
 PUBLIC
  pthread
  ${LINK_THIRDPARTY_LIBS}
)
