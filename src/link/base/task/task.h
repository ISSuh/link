/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_H_
#define LINK_BASE_TASK_TASK_H_

#include <queue>

#include "link/base/time.h"
#include "link/base/callback/callback.h"

namespace nlink {
namespace base {

class Task {
 public:
  Task();
  Task(TaskCallback tasl_callback, TimeTick time);
  Task(const Task& other);
  Task(Task&& other);
  virtual ~Task();

  const TimeTick Timestamp() const;
  bool Runable() const;
  void Run() const;

  Task& operator=(Task&& other);
  bool operator<(const Task& other) const;

 private:
  TaskCallback callback_;
  TimeTick desired_run_time_;
};

using TaskQueue = std::priority_queue<Task>;

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_H_
