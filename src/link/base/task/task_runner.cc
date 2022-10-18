/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_runner.h"

#include <vector>
#include <functional>
#include <utility> 

namespace nlink {
namespace base {

TaskRunner::TaskRunner() = default;

TaskRunner::~TaskRunner() = default;

void TaskRunner::PostTask(TaskCallback callback) {
  PostDelayTask(std::move(callback), TimeTick());
}

TaskRunnerProxy::TaskRunnerProxy(const std::string& label)
  : label_(label) {
}

TaskRunnerProxy::~TaskRunnerProxy() = default;

std::string TaskRunnerProxy::label() const {
  return label_;
}

}  // namespace base
}  // namespace nlink
