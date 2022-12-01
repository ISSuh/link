/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <utility>

#include "link/base/timer.h"

namespace nlink {
namespace base {

Timer::Timer(std::weak_ptr<TaskRunner> task_runner_weak)
  : running_(false),
    desired_run_time_(0),
    task_runner_weak_(task_runner_weak) {
}

Timer::~Timer() = default;

bool Timer::IsRunning() const {
  return running_;
}

void Timer::Start(TaskCallback task, TimeTick delay) {
  if (running_) {
    return;
  }

  user_task_ = std::move(task);

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
  std::shared_ptr<TaskRunner> task_runner = task_runner_weak_.lock();
  if (nullptr == task_runner) {
    return;
  }

  task_runner->PostTask(
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
