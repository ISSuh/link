/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/conqurrent_task_runner.h"

#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace base {

ConcurrentTaskRunner::ConcurrentTaskRunner(const std::string& label, size_t num)
  : TaskRunnerProxy(label),
    number_of_executor_(num),
    running_(true) {
  for (size_t i = 0 ; i < number_of_executor_ ; ++i) {
      std::unique_ptr<TaskExecutor> executor(new TaskExecutor(this));
      executors_.insert({executor->GetWokerId(), std::move(executor)});
  }
}

ConcurrentTaskRunner::~ConcurrentTaskRunner() = default;

void ConcurrentTaskRunner::PostDelayTask(
  const TaskCallback& task_callback, TimeTick delay) {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(Task(task_callback, delay));
  }
  cv_.notify_one();
}

void ConcurrentTaskRunner::StopRunner() {
  LOG(INFO) << "[" << label() << "] " << __func__;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    running_ = false;
  }
  cv_.notify_all();
}

void ConcurrentTaskRunner::WiatForTerminateWorkers() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  for (auto iter = executors_.begin() ; iter != executors_.end();) {
    TaskExecutor* executor = iter->second.get();
    executor->Join();

    iter->second.reset(nullptr);
    executors_.erase(iter++);
  }
}

std::vector<uint64_t> ConcurrentTaskRunner::WorkersIdLists() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  std::vector<uint64_t> id_lists;
  for (auto& excutor : executors_) {
    uint64_t id = excutor.first;
    id_lists.emplace_back(id);
  }
  return id_lists;
}

bool ConcurrentTaskRunner::IsRunning() {
  return running_;
}

void ConcurrentTaskRunner::OnStartWorker(uint64_t id) {
  LOG(TRACE) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void ConcurrentTaskRunner::OnTerminateWorker(uint64_t id) {
  LOG(TRACE) << "[" << label() << "] "
                       << __func__ << " - id : " << id;
}

void ConcurrentTaskRunner::OnStartTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
}

void ConcurrentTaskRunner::OnDidFinishTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
}

Task ConcurrentTaskRunner::NextTask() {
  LOG(TRACE) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);

  if (queue_.empty()) {
    return Task();
  }

  Task task = queue_.top();
  queue_.pop();

  return task;
}

bool ConcurrentTaskRunner::CanWakeUp(uint64_t id) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&](){
        return !IsRunning() || !queue_.empty();
    });
  }
  return IsRunning();
}

}  // namespace base
}  // namespace nlink