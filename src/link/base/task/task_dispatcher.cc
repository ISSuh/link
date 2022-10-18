/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_dispatcher.h"

#include <utility>

#include "link/base/task/task_runner.h"
#include "link/base/logging.h"

namespace nlink {
namespace base {

TaskDispatcher::TaskDispatcher(TaskManager* manager)
  : manager_(manager) {
}

TaskDispatcher::~TaskDispatcher() = default;

void TaskDispatcher::PostTask(
  const std::string& group,
  const std::string& label,
  TaskCallback task) {
  PostDelayTask(group, label, std::move(task), TimeTick());
}

void TaskDispatcher::PostDelayTask(
  const std::string& group,
  const std::string& label,
  TaskCallback task,
  TimeTick delay) {
  TaskRunner* runner = manager_->GetTaskRunner(group, label);
  if (runner == nullptr) {
    return;
  }

  runner->PostDelayTask(std::move(task), delay);
}

}  // namespace base
}  // namespace nlink
