/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/executor.h"

#include <utility>

namespace nlink {
namespace module {

ModuleExecutor::ModuleExecutor(
  std::weak_ptr<base::TaskRunner> task_runner,
  TerminateModuleCallback terminate_module_callback)
  : task_runner_weak_(task_runner),
    terminate_module_callback_(std::move(terminate_module_callback)),
    module_(nullptr) {
}

ModuleExecutor::~ModuleExecutor() = default;

void ModuleExecutor::RunningModule(LinkModule* module) {
  module_ = module;

  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, module = module_]() { module->Initialize(); });
  }
}

void ModuleExecutor::AfterInitialize(const std::string&) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, module = module_]() { module->Process(); });
  }
}

void ModuleExecutor::AfterProcess(const std::string&) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, module = module_]() { module->Terminate(); });
  }
}

void ModuleExecutor::AfterTerminate(const std::string& module_name) {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak_.lock();
  if (task_runner) {
    task_runner->PostTask(
      [this, module_name]() {
        this->terminate_module_callback_(module_name);
      });
  }
}

}  // namespace module
}  // namespace nlink
