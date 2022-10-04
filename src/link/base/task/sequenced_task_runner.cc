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
  LOG(INFO) << "[" << label() << "] " << __func__;
  {
    std::lock_guard<std::mutex> lock(mutex_);

    TimeTick desired_run_time =
      base::TimeTick::Now() + delay;

    // TaskCallback callback(
    //   std::move(task_callback), desired_run_time);

    queue_.emplace(std::move(task_callback), desired_run_time);
  }
  LOG(INFO) << "[" << label() << "] " << __func__ << " - after";
  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  LOG(INFO) << "[" << label() << "] " << __func__;
  running_ = false;
  cv_.notify_all();
}

void SequencedTaskRunner::WiatForTerminateWorkers() {
  LOG(INFO) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return;
  }
  executor_->Join();
}

std::vector<uint64_t> SequencedTaskRunner::WorkersIdLists() {
  LOG(INFO) << "[" << label() << "] " << __func__;
  if (executor_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return {executor_->GetWokerId()};
}

bool SequencedTaskRunner::IsRunning() {
  return running_;
}

void SequencedTaskRunner::OnStartWorker(uint64_t id) {
  LOG(INFO) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnTerminateWorker(uint64_t id) {
  LOG(INFO) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void SequencedTaskRunner::OnStartTask() {
  LOG(INFO) << "[" << label() << "] " << __func__;
}

void SequencedTaskRunner::OnDidFinishTask() {
  LOG(INFO) << "[" << label() << "] " << __func__;
}

Task SequencedTaskRunner::NextTask() {
  LOG(INFO) << "[" << label() << "] " << __func__;

  if (queue_.empty()) {
    return Task();
  }

  TimeTick now = TimeTick::Now();
  if (queue_.top().Timestamp() > now) {
    return Task();
  } else {
    std::lock_guard<std::mutex> lock(mutex_);
    // const Task task = std::move(queue_.top());
    queue_.pop();
    // return std::move(task);
  }
  return Task();
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
