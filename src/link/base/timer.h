/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TIMER_H_
#define LINK_BASE_TIMER_H_

#include "link/base/time.h"
#include "link/base/task/task.h"
#include "link/base/callback/callback.h"
#include "link/base/task/task_runner.h"

namespace nlink {
namespace base {

class Timer {
 public:
  explicit Timer(TaskRunner* task_runner);
  ~Timer();

  bool IsRunning() const;

  void Start(TaskCallback task, TimeTick delay);
  void Stop();

 private:
  void ScheduleUserTask();
  void OnScheduleUserTaskInvoked();
  void RunUserTask();

  bool running_;

  TimeTick desired_run_time_;

  TaskRunner* task_runner_;
  TaskCallback user_task_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TIMER_H_
