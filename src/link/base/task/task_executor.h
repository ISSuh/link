/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_EXECUTOR_H_
#define LINK_BASE_TASK_TASK_EXECUTOR_H_

#include <memory>
#include <thread>
#include <atomic>

#include "link/base/task/task_runner.h"
#include "link/base/task/task_executor_delegate.h"

namespace nlink {
namespace base {

class TaskRunnerProxy;

class TaskExecutor {
 public:
  explicit TaskExecutor(TaskRunnerProxy* task_runner_proxy);
  virtual ~TaskExecutor();

  void Join();
  uint64_t GetWokerId() const;

 private:
  void ExcuteWork();

  void StartWorker();
  void Work();
  void TerminateWorker();

  TaskExecutorDelegate* delegate_;
  std::thread worker_;
  uint64_t id_;
  bool running_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_EXECUTOR_H_
