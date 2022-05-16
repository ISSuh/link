project(link_base)

message("Build Link base")

set(LINK_BASE_SRC
  ${LINK_BASE_PATH}/logging.cc
  ${LINK_BASE_PATH}/time.cc
  ${LINK_BASE_PATH}/json_wrapper.cc

  ${LINK_BASE_PATH}/task/task_runner.cc
  ${LINK_BASE_PATH}/task/sequenced_task_runner.cc
  ${LINK_BASE_PATH}/task/conqurrent_task_runner.cc
  ${LINK_BASE_PATH}/task/task_executor.cc
  ${LINK_BASE_PATH}/task/task_dispatcher.cc
  ${LINK_BASE_PATH}/task_manager.cc

  ${LINK_BASE_PATH}/event/event.cc
  ${LINK_BASE_PATH}/event/event_util.cc
  # ${LINK_BASE_PATH}/event/event_channel.cc
  # ${LINK_BASE_PATH}/event/platform/event_dispatcher_epoll.cc
)

add_library(${LINK_BASE_NAME}
 OBJECT
  ${LINK_BASE_SRC}
)

target_link_libraries(${LINK_BASE_NAME}
 PUBLIC
  pthread
)
