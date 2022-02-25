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

namespace link {
namespace base {

TaskManager::TaskManager()
  : dispatcher_(nullptr) {
}

TaskManager::~TaskManager() {
  if (!runner_map_.empty()) {
    StopAllRunner();
    WaitForTerminateAllTaskRunner();
  }
}

TaskRunner* TaskManager::CreateTaskRunner(
  const std::string& label, TaskRunner::Type type, size_t num) {
  switch (type) {
  case TaskRunner::Type::SEQUENCE:
    return CreateSequencedTaskRunner(label);
  case TaskRunner::Type::CONCURRENT:
    return CreateConqurrentTaskRunner(label, num);
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

void TaskManager::WaitForTerminateTaskRunner(const std::string& label) {
  if (runner_map_.find(label) == runner_map_.end()) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return;
  }

  TaskRunnerProxy* runner = runner_map_.at(label).get();
  if (!runner) {
    return;
  }

  runner->WiatForTerminateWorkers();
}

void TaskManager::WaitForTerminateAllTaskRunner() {
  for (auto&& runner : runner_map_) {
    const std::string label = runner.first;
    WaitForTerminateTaskRunner(label);
  }

  runner_map_.clear();
}

void TaskManager::StopRunner(const std::string& label) {
  if (runner_map_.find(label) == runner_map_.end()) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return;
  }

  TaskRunnerProxy* runner = runner_map_.at(label).get();
  if (!runner || !runner->IsRunning()) {
    return;
  }

  runner->StopRunner();
}

void TaskManager::StopAllRunner() {
  for (const auto& runner : runner_map_) {
    const std::string label = runner.first;
    StopRunner(label);
  }
}

uint32_t TaskManager::NumberOfTaskRunner() const {
  return runner_map_.size();
}

std::vector<std::string> TaskManager::TaskRunnerLabels() const {
  std::vector<std::string> labels;
  for (const auto& runner : runner_map_) {
    const std::string label = runner.first;
    labels.emplace_back(label);
  }
  return labels;
}

TaskRunner* TaskManager::CreateSequencedTaskRunner(const std::string& label) {
  if (runner_map_.find(label) != runner_map_.end()) {
    return dynamic_cast<TaskRunner*>(runner_map_.at(label).get());
  }

  LOG(INFO) << __func__ << " - Create new Sequenced Runner : " << label;

  std::unique_ptr<TaskRunnerProxy> runner =
    std::unique_ptr<TaskRunnerProxy>(new SequencedTaskRunner(label));

  runner_map_.insert({label, std::move(runner)});
  return dynamic_cast<TaskRunner*>(runner_map_.at(label).get());
}

TaskRunner* TaskManager::CreateConqurrentTaskRunner(
  const std::string& label, size_t num) {
  if (runner_map_.find(label) != runner_map_.end()) {
    return dynamic_cast<TaskRunner*>(runner_map_.at(label).get());
  }

  LOG(INFO) << __func__ << " - Create new Conqurrent Runner : " << label;

  std::unique_ptr<TaskRunnerProxy> runner =
    std::unique_ptr<TaskRunnerProxy>(new ConcurrentTaskRunner(label, num));

  runner_map_.insert({label, std::move(runner)});
  return dynamic_cast<TaskRunner*>(runner_map_.at(label).get());
}

TaskRunner* TaskManager::GetTaskRunner(const std::string& label) {
  if (runner_map_.find(label) == runner_map_.end()) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
    return nullptr;
  }
  return runner_map_.at(label).get();
}

}  // namespace base
}  // namespace link
