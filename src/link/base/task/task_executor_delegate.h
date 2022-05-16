/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_EXECUTOR_DELEGATE_H_
#define LINK_BASE_TASK_TASK_EXECUTOR_DELEGATE_H_

#include "link/base/task/task.h"

namespace nlink {
namespace base {

class TaskExecutorDelegate {
 public:
  virtual void OnStartWorker(uint64_t id) = 0;
  virtual void OnTerminateWorker(uint64_t id) = 0;
  virtual void OnStartTask() = 0;
  virtual void OnDidFinishTask() = 0;

  virtual bool CanWakeUp(uint64_t id) = 0;
  virtual Task NextTask() = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_EXECUTOR_DELEGATE_H_
