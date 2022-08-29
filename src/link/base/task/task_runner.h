/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_RUNNER_H_
#define LINK_BASE_TASK_TASK_RUNNER_H_

#include <string>
#include <vector>
#include <functional>

#include "link/base/task/task_executor_delegate.h"
#include "link/base/callback/callback.h"
#include "link/base/time.h"

namespace nlink {
namespace base {

class TaskRunner {
 public:
  enum class Type {
    SEQUENCE,
    CONCURRENT
  };

  virtual void PostTask(TaskCallback callback);
  virtual void PostDelayTask(TaskCallback callback, TimeTick delay) = 0;

 protected:
  TaskRunner();
  virtual ~TaskRunner();
};

class TaskRunnerProxy : public TaskRunner,
                        public TaskExecutorDelegate {
 public:
  explicit TaskRunnerProxy(const std::string& label);
  virtual ~TaskRunnerProxy();

  virtual void StopRunner() = 0;
  virtual void WiatForTerminateWorkers() = 0;
  virtual std::vector<uint64_t> WorkersIdLists() = 0;
  virtual bool IsRunning() = 0;

  std::string label() const;

 private:
  std::string label_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_RUNNER_H_
