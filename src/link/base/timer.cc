/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <iostream>

#include "link/base/timer.h"

namespace nlink {
namespace base {

Timer::Timer(TaskRunner* task_runner)
  : running_(false),
    desired_run_time_(0),
    task_runner_(task_runner) {
}

Timer::~Timer() = default;

bool Timer::IsRunning() const {
  return running_;
}

void Timer::Start(TaskCallback task, TimeTick delay) {
  if (running_) {
    return;
  }

  user_task_ = task;

  if (delay > TimeTick::FromMilliseconds(0)) {
    desired_run_time_ = TimeTick::Now() + delay;
  } else {
    desired_run_time_ = TimeTick();
  }

  running_ = true;
  ScheduleUserTask();
}

void Timer::Stop() {
  running_ = false;
}

void Timer::ScheduleUserTask() {
  task_runner_->PostTask(
    [this](){ this->OnScheduleUserTaskInvoked(); });
}

void Timer::OnScheduleUserTaskInvoked() {
  if (!running_) {
    return;
  }

  TimeTick now = TimeTick::Now();
  if (desired_run_time_ > now) {
    ScheduleUserTask();
    return;
  }

  RunUserTask();
}

void Timer::RunUserTask() {
  Stop();

  if (user_task_) {
    user_task_();
  }
}

}  // namespace base
}  // namespace nlink
