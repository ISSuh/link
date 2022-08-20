/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/task_manager.h"

#include <string>
#include <map>
#include <utility>

#include "link/base/task/sequenced_task_runner.h"
#include "link/base/task/conqurrent_task_runner.h"
#include "link/base/logging.h"
#include "link/base/logger.h"

namespace nlink {
namespace base {

TaskManager::TaskManager()
  : dispatcher_(nullptr) {
}

TaskManager::~TaskManager() {
  if (!runner_groups_.empty()) {
    StopAllRunner();
    WaitForTerminateAllTaskRunner();
  }
}

TaskRunner* TaskManager::CreateTaskRunner(
  const std::string& group,
  const std::string& label,
  TaskRunner::Type type,
  size_t num) {
  switch (type) {
  case TaskRunner::Type::SEQUENCE:
    return CreateSequencedTaskRunner(group, label);
  case TaskRunner::Type::CONCURRENT:
    return CreateConqurrentTaskRunner(group, label, num);
  default:
    return nullptr;
  }
}

TaskDispatcher* TaskManager::GetTaskDispatcher() {
  if (dispatcher_ == nullptr) {
    dispatcher_.reset(new TaskDispatcher(this));
  }
  return dispatcher_.get();
}

void TaskManager::WaitForTerminateTaskRunner(
  const std::string& group, const std::string& label) {
  if (!HasTaskRunner(group, label)) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return;
  }

  auto& runners = runner_groups_.at(group);
  TaskRunnerProxy* runner = runners.at(label).get();
  if (!runner) {
    return;
  }

  runner->WiatForTerminateWorkers();
}

void TaskManager::WaitForTerminateTaskRunnerGroup(const std::string& group) {
  if (!HasTaskRunnerGroup(group)) {
    return;
  }

  auto& runners = runner_groups_.at(group);
  for (const auto& runner : runners) {
    const std::string label = runner.first;
    WaitForTerminateTaskRunner(group, label);
  }
  runners.clear();
}

void TaskManager::WaitForTerminateAllTaskRunner() {
  for (auto& group : runner_groups_) {
    std::string group_name = group.first;
    WaitForTerminateTaskRunnerGroup(group_name);
  }
  runner_groups_.clear();
}

void TaskManager::StopRunner(
  const std::string& group, const std::string& label) {
  if (!HasTaskRunner(group, label)) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return;
  }

  auto& runners = runner_groups_.at(group);
  TaskRunnerProxy* runner = runners.at(label).get();
  if (!runner || !runner->IsRunning()) {
    return;
  }

  runner->StopRunner();
}

void TaskManager::StopRunnerGroup(const std::string& group) {
  if (!HasTaskRunnerGroup(group)) {
    return;
  }

  auto& runners = runner_groups_.at(group);
  for (const auto& runner : runners) {
    const std::string label = runner.first;
    StopRunner(group, label);
  }
}

void TaskManager::StopAllRunner() {
  for (const auto& group : runner_groups_) {
    std::string group_name = group.first;
    StopRunnerGroup(group_name);
  }
}

uint32_t TaskManager::NumberOfTaskRunnerByGroup(
  const std::string& group) const {
  if (!HasTaskRunnerGroup(group)) {
    return 0;
  }

  auto& runners = runner_groups_.at(group);
  return runners.size();
}

uint32_t TaskManager::NumberOfTaskRunner() const {
  uint32_t count = 0;
  for (const auto& group : runner_groups_) {
    std::string group_label = group.first;
    count += NumberOfTaskRunnerByGroup(group_label);
  }
  return count;
}

std::map<std::string, std::string> TaskManager::TaskRunnerLabels() const {
  std::map<std::string, std::string> labels_by_group;
  for (const auto& group : runner_groups_) {
    std::string group_name = group.first;
    const RunnerMap& runners = group.second;
    for (const auto& runner : runners) {
      const std::string label = runner.first;
      labels_by_group.insert({group_name, label});
    }
  }
  return labels_by_group;
}

TaskManager::TaskThreadIdByLabel TaskManager::TaskThreadIds() const {
  TaskThreadIdByLabel thread_ids;
  for (const auto& group : runner_groups_) {
    const RunnerMap& runners = group.second;
    for (const auto& runner : runners) {
      const std::string label = runner.first;
      std::vector<uint64_t> ids_ = runner.second->WorkersIdLists();

      for (const auto id : ids_) {
        thread_ids.emplace_back(label, id);
      }
    }
  }
  return thread_ids;
}

bool TaskManager::HasTaskRunnerGroup(const std::string& group) const {
  return runner_groups_.find(group) != runner_groups_.end();
}

bool TaskManager::HasTaskRunner(
  const std::string& group, const std::string& label) const {
  if (!HasTaskRunnerGroup(group)) {
    return false;
  }

  auto& runners = runner_groups_.at(group);
  return runners.find(label) != runners.end();
}

bool TaskManager::IsTaskRunnerRunning(
  const std::string& group, const std::string& label) const {
  if (!HasTaskRunner(group, label)) {
    return false;
  }

  auto& runners = runner_groups_.at(group);
  return runners.at(label)->IsRunning();
}

TaskRunner* TaskManager::CreateSequencedTaskRunner(
  const std::string& group, const std::string& label) {
  if (HasTaskRunner(group, label)) {
    auto& runners = runner_groups_.at(group);
    return dynamic_cast<TaskRunner*>(runners.at(label).get());
  }

  LOG(INFO) << __func__ << " - Create new Sequenced Runner : "
                        << group << "/" << label;

  std::unique_ptr<TaskRunnerProxy> runner =
    std::unique_ptr<TaskRunnerProxy>(new SequencedTaskRunner(label));

  return InsertRunner(std::move(runner), group, label);
}

TaskRunner* TaskManager::CreateConqurrentTaskRunner(
  const std::string& group, const std::string& label, size_t num) {
  if (HasTaskRunner(group, label)) {
    auto& runners = runner_groups_.at(group);
    return dynamic_cast<TaskRunner*>(runners.at(label).get());
  }

  LOG(INFO) << __func__ << " - Create new Conqurrent Runner : "
                        << group << "/" << label;

  std::unique_ptr<TaskRunnerProxy> runner =
    std::unique_ptr<TaskRunnerProxy>(new ConcurrentTaskRunner(label, num));

  return InsertRunner(std::move(runner), group, label);
}

TaskRunner* TaskManager::InsertRunner(
  std::unique_ptr<TaskRunnerProxy> runner,
  const std::string& group,
  const std::string& label) {
  if (HasTaskRunnerGroup(group)) {
    auto& runners = runner_groups_.at(group);
    runners.insert({label, std::move(runner)});
  } else {
    RunnerMap runners;
    runners.insert({label, std::move(runner)});
    runner_groups_.insert({group, std::move(runners)});
  }

  CreateLoggerForNewTaskRunner(label);
  auto& runners = runner_groups_.at(group);
  return dynamic_cast<TaskRunner*>(runners.at(label).get());
}

TaskRunner* TaskManager::GetTaskRunner(
  const std::string& group, const std::string& label) {
  if (!HasTaskRunner(group, label)) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return nullptr;
  }

  auto& runners = runner_groups_.at(group);
  return runners.at(label).get();
}

void TaskManager::CreateLoggerForNewTaskRunner(const std::string& label) {
  base::LoggerManager::Instance()->CreateLogger(label);
}

}  // namespace base
}  // namespace nlink
