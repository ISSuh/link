/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/base/time.h>
#include <link/base/timer.h>
#include <link/base/task_manager.h>
#include <link/base/logger.h>

using namespace nlink;

class TimerTask {
  virtual void Task() = 0;
};

class TimerTaskkMock : public TimerTask {
 public:
  MOCK_METHOD0(Task, void());
};

void WaitForTimer(base::Timer* timer) {
  while (timer->IsRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

TEST(Timer, timer_test) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner =
    task_manager->CreateTaskRunner(
      "test", "test", base::TaskRunner::Type::SEQUENCE);

  base::Timer timer(task_runner);
  TimerTaskkMock mock_task;

  EXPECT_CALL(mock_task, Task()).Times(1);

  base::TimeTick before_tick = base::TimeTick::Now();
  base::TimeTick after_tick;
  timer.Start(
    [&mock_task, &after_tick]() {
      after_tick = base::TimeTick::Now();
      mock_task.Task();
    },
    base::TimeTick::FromMilliseconds(1000));

  WaitForTimer(&timer);

  auto tick = after_tick - before_tick;
  EXPECT_GE(tick.Tick(), 1000);

  task_manager->StopAllRunner();
}
