project(link_base)

# message("Build Link base")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_LINK})

set(LINK_BASE_NAME link_base)
set(PROJECT_LINK_SRC_DIR ${PROJECT_SOURCE_DIR}/src/link/base)

set(LINK_BASE_SRC
  ${PROJECT_LINK_SRC_DIR}/logging.cc
  ${PROJECT_LINK_SRC_DIR}/time.cc

  ${PROJECT_LINK_SRC_DIR}/task/task_runner.cc
  ${PROJECT_LINK_SRC_DIR}/task/sequenced_task_runner.cc
  ${PROJECT_LINK_SRC_DIR}/task/conqurrent_task_runner.cc
  ${PROJECT_LINK_SRC_DIR}/task/task_executor.cc
  ${PROJECT_LINK_SRC_DIR}/task/task_dispatcher.cc
  ${PROJECT_LINK_SRC_DIR}/task_manager.cc
)

# add_library(${LINK_BASE_NAME}
#  STATIC
#   ${LINK_BASE_SRC}
# )

# target_link_libraries(${LINK_BASE_NAME}
#  PUBLIC
#   pthread
# )
