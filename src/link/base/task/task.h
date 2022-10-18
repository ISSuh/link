/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_H_
#define LINK_BASE_TASK_TASK_H_

#include <vector>
#include <queue>

#include "link/base/time.h"
#include "link/base/callback/callback.h"

namespace nlink {
namespace base {

class Task {
 public:
  Task();
  Task(TaskCallback tasl_callback, TimeTick time);
  Task(Task&& other);
  virtual ~Task();

  const TimeTick Timestamp() const;
  bool Runable() const;
  void Run() const;

  Task& operator=(Task&& other);
  bool operator<(const Task& other) const;

  Task(const Task& other) = delete;
  Task& operator=(const Task& other) = delete;

 private:
  TaskCallback callback_;
  TimeTick desired_run_time_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_H_
