/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_dispatcher.h"

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
  const TaskCallback& task) {
  PostDelayTask(group, label, task, TimeTick());
}

void TaskDispatcher::PostDelayTask(
  const std::string& group,
  const std::string& label,
  const TaskCallback& task,
  TimeTick delay) {
  TaskRunner* runner = manager_->GetTaskRunner(group, label);
  if (runner == nullptr) {
    return;
  }

  runner->PostDelayTask(task, delay);
}

}  // namespace base
}  // namespace nlink
