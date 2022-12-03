project(link_base)

message("Build Link base")

FILE(GLOB base ${LINK_BASE_PATH}/*.cc)
FILE(GLOB base_task ${LINK_BASE_PATH}/task/*.cc)
FILE(GLOB base_event ${LINK_BASE_PATH}/event/*.cc)

if(LINK_USE_ASIO)
  FILE(GLOB base_event_platform ${LINK_BASE_PATH}/event/platform/asio/*.cc)
elseif(LINK_USE_EPOLL)
  FILE(GLOB base_event_platform ${LINK_BASE_PATH}/event/platform/epoll/*.cc)
elseif(LINK_USE_KQUEUE)
  # FILE(GLOB base_event_platform ${LINK_BASE_PATH}/event/platform/kqueue/*.cc)
else()
  message("need setting event platform")
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
