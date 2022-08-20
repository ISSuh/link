/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/base/time.h>
#include <link/base/task_manager.h>
#include <link/base/logger.h>

using namespace nlink;

class UserTask {
  virtual void Task1() = 0;
  virtual void Task2(int32_t) = 0;
  virtual void Task3(int32_t, bool) = 0;
};

class UserTaskkMock : public UserTask {
 public:
  MOCK_METHOD0(Task1, void());
  MOCK_METHOD1(Task2, void(int32_t));
  MOCK_METHOD2(Task3, void(int32_t, bool));
};

void WaitForTask(
  std::atomic_int32_t* finished_task_num, int32_t total_task_num) {
  while (*finished_task_num < total_task_num) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

TEST(SequencedTaskRunner, run_task_test) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner =
    task_manager->CreateTaskRunner(
      "test", "sequence_task", base::TaskRunner::Type::SEQUENCE);

  UserTaskkMock mock_task;
  std::atomic_int32_t finished_task_num(0);

  EXPECT_CALL(mock_task, Task1()).Times(1);

  task_runner->PostTask(
    [&]() {
      mock_task.Task1();
      finished_task_num.fetch_add(1);
    });

  WaitForTask(&finished_task_num, 1);

  task_manager->StopAllRunner();
}

TEST(SequencedTaskRunner, run_delayed_task_test) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner =
    task_manager->CreateTaskRunner(
      "test", "sequence_task", base::TaskRunner::Type::SEQUENCE);

  UserTaskkMock mock_task;
  std::atomic_int32_t finished_task_num(0);

  // post task to task3 -> task2 -> task1
  // but task will be runed task1 -> task2 -> task3
  testing::Expectation expect_task1 =
    EXPECT_CALL(mock_task, Task1()).Times(1);

  testing::Expectation expect_task2 =
    EXPECT_CALL(mock_task, Task2(testing::_)).Times(1).After(expect_task1);

  testing::Expectation expect_task3 =
    EXPECT_CALL(mock_task, Task3(testing::_, testing::_))
      .Times(1).After(expect_task2);

  task_runner->PostDelayTask(
    [&]() {
      mock_task.Task3(10, false);
      finished_task_num.fetch_add(1);
    },
    base::TimeTick::FromMilliseconds(200));

  task_runner->PostDelayTask(
    [&]() {
      mock_task.Task2(5);
      finished_task_num.fetch_add(1);
    },
    base::TimeTick::FromMilliseconds(100));

  task_runner->PostTask(
    [&]() {
      mock_task.Task1();
      finished_task_num.fetch_add(1);
    });

  WaitForTask(&finished_task_num, 3);

  task_manager->StopAllRunner();
}

TEST(ConcurrentTaskRunner, run_task_test) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner =
    task_manager->CreateTaskRunner(
      "test", "concurrent_task", base::TaskRunner::Type::CONCURRENT, 2);

  UserTaskkMock mock_task;
  std::atomic_int32_t finished_task_num(0);

  EXPECT_CALL(mock_task, Task1()).Times(1);
  EXPECT_CALL(mock_task, Task2(testing::_)).Times(1);

  task_runner->PostTask(
    [&]() {
      std::cout << "func task1" << std::endl;
      mock_task.Task1();
      finished_task_num.fetch_add(1);
    });

  task_runner->PostTask(
    [&]() {
      std::cout << "func task2" << std::endl;
      mock_task.Task2(0);
      finished_task_num.fetch_add(1);
    });

  WaitForTask(&finished_task_num, 2);

  task_manager->StopAllRunner();
}

TEST(ConcurrentTaskRunner, run_delayed_task_test) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner =
    task_manager->CreateTaskRunner(
      "test", "concurrent_task", base::TaskRunner::Type::CONCURRENT, 3);

  UserTaskkMock mock_task;
  std::atomic_int32_t finished_task_num(0);

  // post task to task3 -> task2 -> task1
  // but task will be runed task1 -> task2 -> task3
  testing::Expectation expect_task1 =
    EXPECT_CALL(mock_task, Task1()).Times(1);

  testing::Expectation expect_task2 =
    EXPECT_CALL(mock_task, Task2(testing::_)).Times(1).After(expect_task1);

  testing::Expectation expect_task3 =
    EXPECT_CALL(mock_task, Task3(testing::_, testing::_))
      .Times(1).After(expect_task2);

  task_runner->PostDelayTask(
    [&]() {
      mock_task.Task3(10, false);
      finished_task_num.fetch_add(1);
    },
    base::TimeTick::FromMilliseconds(200));

  task_runner->PostDelayTask(
    [&]() {
      mock_task.Task2(5);
      finished_task_num.fetch_add(1);
    },
    base::TimeTick::FromMilliseconds(100));

  task_runner->PostTask(
    [&]() {
      mock_task.Task1();
      finished_task_num.fetch_add(1);
    });

  WaitForTask(&finished_task_num, 3);

  task_manager->StopAllRunner();
}
