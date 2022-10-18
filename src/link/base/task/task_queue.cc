/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task_queue.h"

#include <utility>
#include <algorithm>

namespace nlink {
namespace base {

struct Compare {
  bool operator()(const Task& a, const Task& b) const {
      return a.Timestamp() > b.Timestamp();
  }
};

TaskQueue::TaskQueue()
  : task_queue_(0) {
}

void TaskQueue::Push(base::Task task) {
  std::lock_guard<std::mutex> lock(mutex_);
  task_queue_.push_back(std::move(task));
  std::make_heap(task_queue_.begin(), task_queue_.end(), Compare());
}

void TaskQueue::Pop() {
  std::lock_guard<std::mutex> lock(mutex_);
  std::pop_heap(task_queue_.begin(), task_queue_.end());
  task_queue_.pop_back();
}

Task TaskQueue::Top() {
  return std::move(task_queue_.front());
}

TimeTick TaskQueue::TimeOnTop() const {
  if (task_queue_.empty()) {
    return TimeTick();
  }
  return task_queue_.front().Timestamp();
}

bool TaskQueue::Empty() const {
  return task_queue_.empty();
}

size_t TaskQueue::Size() const {
  return task_queue_.size();
}

}  // namespace base
}  // namespace nlink
