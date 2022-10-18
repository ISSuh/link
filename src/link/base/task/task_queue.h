/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_QUEUE_H_
#define LINK_BASE_TASK_TASK_QUEUE_H_

#include <vector>
#include <mutex>

#include "link/base/task/task.h"

namespace nlink {
namespace base {

class TaskQueue {
 public:
  TaskQueue();
  ~TaskQueue() = default;

  void Push(base::Task task);
  void Pop();
  Task Top();

  TimeTick TimeOnTop() const;

  bool Empty() const;
  size_t Size() const;

 private:
  std::vector<base::Task> task_queue_;
  std::mutex mutex_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_QUEUE_H_
