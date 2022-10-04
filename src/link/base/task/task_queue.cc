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

TaskQueuet::TaskQueuet()
  : task_queue_(0) {
}

void TaskQueuet::Push(base::Task task) {
  task_queue_.push_back(std::move(task));
  std::make_heap(task_queue_.begin(), task_queue_.end());
}

void TaskQueuet::Pop() {
  std::pop_heap(task_queue_.begin(), task_queue_.end());
  task_queue_.pop_back();
}

Task TaskQueuet::Top() const {
  return std::move(task_queue_[task_queue_.size() - 1]);
}

}  // namespace base
}  // namespace nlink
