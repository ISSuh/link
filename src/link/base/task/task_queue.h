/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_QUEUE_H_
#define LINK_BASE_TASK_TASK_QUEUE_H_

#include <vector>

#include "link/base/task/task.h"

namespace nlink {
namespace base {

class TaskQueuet {
 public:
  TaskQueuet();
  ~TaskQueuet() = default;

  void Push(base::Task task);
  void Pop();
  Task Top() const;

 private:
  std::vector<base::Task> task_queue_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_QUEUE_H_
