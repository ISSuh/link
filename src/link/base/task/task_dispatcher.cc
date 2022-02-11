/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_dispatcher.h"

#include "link/base/task/task_runner.h"
#include "link/base/logging.h"

namespace link {
namespace base {

TaskDispatcher::TaskDispatcher(TaskManager* manager)
  : manager_(manager) {
}

TaskDispatcher::~TaskDispatcher() = default;

void TaskDispatcher::PostTask(const std::string& label,
              const TaskCallback& task_callback) {
  PostDelayTask(label, task_callback, TimeTick());
}

void TaskDispatcher::PostDelayTask(const std::string& label,
              const TaskCallback& task_callback, TimeTick delay) {
  TaskRunner* runner = manager_->GetTaskRunner(label);
  if (runner == nullptr) {
    return;
  }

  runner->PostDelayTask(task_callback, delay);
}

}  // namespace base
}  // namespace link
