/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_H_
#define LINK_BASE_TASK_TASK_H_

#include <vector>
#include <queue>
#include <functional>

#include "link/base/time.h"
#include "link/base/callback/callback.h"

namespace nlink {
namespace base {

struct Task {
  Task() = default;
  Task(const TaskCallback& tasl_callback, TimeTick time)
    : callback(tasl_callback), desired_run_time(time) {}
  ~Task() = default;

  TaskCallback callback;
  TimeTick desired_run_time;

  bool operator<(const Task& other) const {
        return desired_run_time > other.desired_run_time;
  }
};

using TaskQueue = std::priority_queue<Task>;

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_H_
