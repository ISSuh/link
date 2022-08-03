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
#include <utility> 

#include "link/base/task/task_runner.h"
#include "link/base/task/task_dispatcher.h"
#include "link/base/macro.h"

namespace nlink {
namespace base {

class TaskDispatcher;

class TaskManager : public std::enable_shared_from_this<TaskManager> {
 public:
  using TaskThreadIdByLabel = std::vector<std::pair<std::string, uint64_t>>;
  using RunnerMap = std::map<std::string, std::unique_ptr<TaskRunnerProxy>>;

  TaskManager();
  ~TaskManager();

  TaskRunner* CreateTaskRunner(
    const std::string& group,
    const std::string& label,
    TaskRunner::Type type,
    size_t num = 0);
  TaskDispatcher* GetTaskDispatcher();

  void WaitForTerminateTaskRunner(
    const std::string& group, const std::string& label);
  void WaitForTerminateTaskRunnerGroup(const std::string& group);
  void WaitForTerminateAllTaskRunner();

  void StopRunner(const std::string& group, const std::string& label);
  void StopRunnerGroup(const std::string& group);
  void StopAllRunner();

  uint32_t NumberOfTaskRunnerByGroup(const std::string& group) const;
  uint32_t NumberOfTaskRunner() const;
  std::map<std::string, std::string> TaskRunnerLabels() const;
  TaskThreadIdByLabel TaskThreadIds() const;

  bool HasTaskRunnerGroup(const std::string& group) const;
  bool HasTaskRunner(const std::string& group, const std::string& label) const;
  bool IsTaskRunnerRunning(
    const std::string& group, const std::string& label) const;

 private:
  friend TaskDispatcher;

  TaskRunner* CreateSequencedTaskRunner(
    const std::string& group, const std::string& label);
  TaskRunner* CreateConqurrentTaskRunner(
    const std::string& group, const std::string& label, size_t num);
  TaskRunner* InsertRunner(
    std::unique_ptr<TaskRunnerProxy> runner,
    const std::string& group,
    const std::string& label);

  TaskRunner* GetTaskRunner(
    const std::string& group, const std::string& label);
  void CreateLoggerForNewTaskRunner(const std::string& label);

  std::map<std::string, RunnerMap> runner_groups_;
  std::unique_ptr<TaskDispatcher> dispatcher_;

  DISAALOW_COPY_AND_ASSIGN(TaskManager)
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_MANAGER_H_
