/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_CONQURRENT_TASK_RUNNER_H_
#define LINK_BASE_TASK_CONQURRENT_TASK_RUNNER_H_

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "link/base/task/task_runner.h"
#include "link/base/task/task_executor.h"
#include "link/base/callback/callback.h"
#include "link/base/time.h"

namespace link {
namespace base {

class TaskExecutor;

class ConcurrentTaskRunner final : public TaskRunnerProxy {
 public:
  explicit ConcurrentTaskRunner(const std::string& label, size_t num);
  virtual ~ConcurrentTaskRunner();

  // TaksRunner
  void PostDelayTask(
    const TaskCallback& task_callback, TimeTick delay) override;

  // TaksRunnerProxy
  void StopRunner() override;
  void WiatForTerminateWorkers() override;

  std::vector<uint64_t> WorkersIdLists() override;
  bool IsRunning() override;

  // WokerThread::Delegate
  void OnStartWorker(uint64_t id) override;
  void OnTerminateWorker(uint64_t id) override;
  void OnStartTask() override;
  void OnDidFinishTask() override;

  Task NextTask() override;
  bool CanWakeUp(uint64_t id) override;

 private:
  size_t number_of_executor_;
  std::map<uint64_t, std::unique_ptr<TaskExecutor>> executors_;
  TaskQueue queue_;

  bool running_;
  std::condition_variable cv_;
  std::mutex mutex_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_TASK_CONQURRENT_TASK_RUNNER_H_
