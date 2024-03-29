/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_executor.h"

#include <utility>

#include "link/base/task/task.h"
#include "link/base/logging.h"
#include "link/base/thread.h"

namespace nlink {
namespace base {

TaskExecutor::TaskExecutor(TaskRunnerProxy* task_runner_proxy)
  : delegate_(task_runner_proxy),
    worker_(std::thread(&TaskExecutor::ExcuteWork, this)),
    id_(std::hash<std::thread::id>{}(worker_.get_id())),
    running_(true) {
}

TaskExecutor::~TaskExecutor() {
  Join();
}

void TaskExecutor::Join() {
  LOG(TRACE) << __func__;
  if (worker_.joinable()) {
    worker_.join();
  }
}

uint64_t TaskExecutor::GetWokerId() const {
  return id_;
}

void TaskExecutor::ExcuteWork() {
  StartWorker();
  Work();
  TerminateWorker();
}

void TaskExecutor::StartWorker() {
  running_ = true;
  delegate_->OnStartWorker(id_);
}

void TaskExecutor::Work() {
  while (delegate_->CanWakeUp(id_)) {
    Task task = delegate_->NextTask();
    if (!task.Runable()) {
      continue;
    }

    delegate_->OnStartTask();
    task.Run();
    delegate_->OnDidFinishTask();
  }
}

void TaskExecutor::TerminateWorker() {
  LOG(INFO) << __func__;
  delegate_->OnTerminateWorker(id_);
  running_ = false;
}

}  // namespace base
}  // namespace nlink
