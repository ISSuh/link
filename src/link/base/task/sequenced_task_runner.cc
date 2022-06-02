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

SequencedTaskRunner::~SequencedTaskRunner() {
  VLOG(2) << "[" << label() << "] " << __func__;
}

void SequencedTaskRunner::PostDelayTask(
  const TaskCallback& task_callback, TimeTick delay) {
  VLOG(2) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(Task(task_callback, delay));
  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  VLOG(2) << "[" << label() << "] " << __func__;
  running_ = false;
  cv_.notify_all();
}

void SequencedTaskRunner::WiatForTerminateWorkers() {
  VLOG(2) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return;
  }
  executor_->Join();
}

std::vector<uint64_t> SequencedTaskRunner::WorkersIdLists() {
  VLOG(2) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return {executor_->GetWokerId()};
}

bool SequencedTaskRunner::IsRunning() {
  return running_;
}

void SequencedTaskRunner::OnStartWorker(uint64_t id) {
  VLOG(2) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnTerminateWorker(uint64_t id) {
  VLOG(2) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnStartTask() {
  VLOG(2) << "[" << label() << "] " << __func__;
}

void SequencedTaskRunner::OnDidFinishTask() {
  VLOG(2) << "[" << label() << "] " << __func__;
}

Task SequencedTaskRunner::NextTask() {
  VLOG(2) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);

  if (queue_.empty()) {
    return Task();
  }

  Task task = queue_.top();
  queue_.pop();

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
