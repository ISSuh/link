/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_MANAGER_H_
#define LINK_BASE_TASK_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "link/base/task/task_runner.h"
#include "link/base/task/task_dispatcher.h"

namespace link {
namespace base {

class TaskDispatcher;

class TaskManager {
 public:
  TaskManager();
  ~TaskManager();

  TaskRunner* CreateTaskRunner(
    const std::string& label, TaskRunner::Type type, size_t num = 0);
  TaskDispatcher* GetTaskDispatcher();

  void WaitForTerminateTaskRunner(const std::string& task_runner_label);
  void WaitForTerminateAllTaskRunner();

  void StopRunner(const std::string& label);
  void StopAllRunner();

  uint32_t NumberOfTaskRunner() const;
  std::vector<std::string> TaskRunnerLabels() const;

  TaskManager(TaskManager&) = delete;
  TaskManager& operator=(const TaskManager&) = delete;

 private:
  friend TaskDispatcher;

  TaskRunner* CreateSequencedTaskRunner(const std::string& label);
  TaskRunner* CreateConqurrentTaskRunner(const std::string& label, size_t num);
  TaskRunner* GetTaskRunner(const std::string& label);

  std::map<std::string, std::unique_ptr<TaskRunnerProxy>> runner_map_;
  std::unique_ptr<TaskDispatcher> dispatcher_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_TASK_MANAGER_H_
