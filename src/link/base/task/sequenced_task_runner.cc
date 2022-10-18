/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/sequenced_task_runner.h"

#include <utility>

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
  TaskCallback task_callback, TimeTick delay) {
  LOG(TRACE) << "[" << label() << "] " << __func__;

  TimeTick desired_run_time = base::TimeTick::Now() + delay;
  Task task(std::move(task_callback), desired_run_time);
  queue_.Push(std::move(task));

  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  running_.store(false);
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
  return running_.load();
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

  if (queue_.Empty()) {
    return Task();
  }

  TimeTick now = TimeTick::Now();
  if (queue_.TimeOnTop() < now) {
    Task task = std::move(queue_.Top());
    queue_.Pop();
    return task;
  }
  return Task();
}

bool SequencedTaskRunner::CanWakeUp(uint64_t id) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&](){
        return !IsRunning() || !queue_.Empty();
    });
    lock.unlock();
  }
  return IsRunning();
}

}  // namespace base
}  // namespace nlink
