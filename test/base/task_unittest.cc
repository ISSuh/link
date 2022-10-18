/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/base/task/task.h>
#include <link/base/task/task_queue.h>
#include <link/base/time.h>

using namespace nlink;

struct FreeTaskFunction {
    virtual void Invoke(int32_t) = 0;
};

struct FreeTaskFunctionMock : public FreeTaskFunction {
 public:
    MOCK_METHOD1(Invoke, void(int32_t));
};


void TestTaskCallback(int32_t a) {
  FreeTaskFunctionMock free_task_func_mock;
  return free_task_func_mock.Invoke(a);
}

class UserTaskCallbacks {
  virtual void TaskCallback1() = 0;
  virtual void TaskCallback2(const std::string&) = 0;
};

class UserTaskCallbacksMock : public UserTaskCallbacks {
 public:
  MOCK_METHOD0(TaskCallback1, void());
  MOCK_METHOD1(TaskCallback2, void(const std::string&));
};

TEST(Task, make_task) {
  UserTaskCallbacksMock callback_mock;

  {
    base::Task task;
    EXPECT_FALSE(task.Runable());
  }

  {
    base::TaskCallback callback =
      [&]() {
        callback_mock.TaskCallback2("test");
      };

    base::Task task(std::move(callback), base::TimeTick::Now());

    EXPECT_FALSE(callback);
    EXPECT_TRUE(task.Runable());
    EXPECT_CALL(callback_mock, TaskCallback2(testing::_));

    task.Run();
  }
}

TEST(Task, move_task) {
  UserTaskCallbacksMock callback_mock;

  {
    base::TaskCallback callback =
      [&]() {
        callback_mock.TaskCallback1();
      };

    base::Task task(std::move(callback), base::TimeTick::Now());

    EXPECT_FALSE(callback);
    EXPECT_TRUE(task.Runable());
    EXPECT_CALL(callback_mock, TaskCallback1());

    task.Run();

    base::Task moved_task;
    EXPECT_FALSE(moved_task.Runable());

    moved_task = std::move(task);
    EXPECT_FALSE(task.Runable());
    EXPECT_TRUE(moved_task.Runable());
    EXPECT_CALL(callback_mock, TaskCallback1());

    moved_task.Run();
  }
}

TEST(Task, task_queue) {
  UserTaskCallbacksMock callback_mock;

  {
    base::TaskQueue queue;

    base::TaskCallback callback1 =
      [&]() {
        callback_mock.TaskCallback1();
        std::cout << "TaskCallback2" << std::endl;
      };

    base::TaskCallback callback2 =
      [&]() {
        callback_mock.TaskCallback2("test");
        std::cout << "TaskCallback2" << std::endl;
      };

    base::Task temp_task1(
      std::move(callback1),
      base::TimeTick::Now() + base::TimeTick::FromMilliseconds(10000));

    queue.Push(std::move(temp_task1));

    base::Task temp_task2(
      std::move(callback2),
      base::TimeTick::Now());

    queue.Push(std::move(temp_task2));

    EXPECT_EQ(2, queue.Size());

    testing::Expectation expect_task2 =
      EXPECT_CALL(callback_mock, TaskCallback2(testing::_)).Times(1);

    testing::Expectation expect_task1 =
      EXPECT_CALL(callback_mock, TaskCallback1()).Times(1).After(expect_task2);

    base::Task task2 = std::move(queue.Top());
    queue.Pop();
    EXPECT_TRUE(task2.Runable());
    task2.Run();

    base::Task task1 = std::move(queue.Top());
    queue.Pop();
    EXPECT_TRUE(task1.Runable());
    task1.Run();

    EXPECT_TRUE(queue.Empty());
  }
}
