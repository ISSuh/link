/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_H_
#define LINK_BASE_TASK_TASK_H_

#include <stddef.h>
#include <functional>

#include <queue>

#include "link/base/time.h"
#include "link/base/callback/callback.h"

namespace nlink {
namespace base {

struct Task {
  Task() = default;
  Task(const TaskCallback& tasl_callback, TimeTick delay)
    : callback(tasl_callback), delay(delay) {}
  ~Task() = default;

  TaskCallback callback;
  TimeTick delay;

  bool operator<(const Task& s) const {
        return delay < s.delay;
  }
};

using TaskQueue = std::queue<Task>;

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_H_
