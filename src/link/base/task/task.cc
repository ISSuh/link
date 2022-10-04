/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task/task.h"

#include <utility>

namespace nlink {
namespace base {

Task::Task()
  : callback_(), desired_run_time_(0) {
}

Task::Task(TaskCallback tasl_callback, TimeTick time)
  : callback_(std::move(tasl_callback)), desired_run_time_(time) {}

// Task::Task(const Task& other)
//   : callback_(std::move(other.callback_)),
//     desired_run_time_(other.desired_run_time_) {
// }

// Task::Task(Task&& other)
//   : callback_(std::move(other.callback_)),
//     desired_run_time_(other.desired_run_time_) {
// }

Task::~Task() = default;

const TimeTick Task::Timestamp() const {
  return desired_run_time_;
}

bool Task::Runable() const {
  return callback_ ? true : false;
}

void Task::Run() const {
  if (Runable()) {
    callback_();
  }
}

// Task& Task::operator=(Task&& other) {
//   if (&other == this) {
//     return *this;
//   }

//   callback_ = std::move(other.callback_);
//   desired_run_time_ = other.desired_run_time_;
//   return *this;
// }

bool Task::operator<(const Task& other) const {
  return desired_run_time_ > other.desired_run_time_;
}


}  // namespace base
}  // namespace nlink
