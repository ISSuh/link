/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/sequenced_task_runner.h"

#include "link/base/logging.h"

namespace nlink {
namespace base {

SequencedTaskRunner::SequencedTaskRunner(const std::string& label)
  : TaskRunnerProxy(label),
    running_(true) {
  executor_ = std::unique_ptr<TaskExecutor>(
      new TaskExecutor(dynamic_cast<TaskRunnerProxy*>(this)));
}

SequencedTaskRunner::~SequencedTaskRunner() = default;

void SequencedTaskRunner::PostDelayTask(
  const TaskCallback& task_callback, TimeTick delay) {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  {
    std::lock_guard<std::mutex> lock(mutex_);

    TimeTick desired_run_time =
      base::TimeTick::Now() + delay;
    queue_.emplace(task_callback, desired_run_time);
  }
  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  running_ = false;
  cv_.notify_all();
}

void SequencedTaskRunner::WiatForTerminateWorkers() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return;
  }
  executor_->Join();
}

std::vector<uint64_t> SequencedTaskRunner::WorkersIdLists() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return {executor_->GetWokerId()};
}

bool SequencedTaskRunner::IsRunning() {
  return running_;
}

void SequencedTaskRunner::OnStartWorker(uint64_t id) {
  LOG(TRACE) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnTerminateWorker(uint64_t id) {
  LOG(TRACE) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnStartTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
}

void SequencedTaskRunner::OnDidFinishTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
}

Task SequencedTaskRunner::NextTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;

  if (queue_.empty()) {
    return Task();
  }

  Task task = queue_.top();

  TimeTick now = TimeTick::Now();
  if (task.desired_run_time > now) {
    return Task();
  } else {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.pop();
  }
  return task;
}

bool SequencedTaskRunner::CanWakeUp(uint64_t id) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&](){
        return !IsRunning() || !queue_.empty();
    });
    lock.unlock();
  }
  return IsRunning();
}

}  // namespace base
}  // namespace nlink
