/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/controller.h"

#include <utility>

#include "link/base/logging.h"

namespace link {
namespace module {

const char* kModuleTaskRunnerName = "ModuleTaskRunner";

ModuleController::ModuleController(base::TaskManager* task_manager)
  : task_manager_(task_manager) {
}

ModuleController::~ModuleController() {
}

bool ModuleController::LoadingModule(const std::vector<Specification>& specs) {
  if (specs.empty()) {
    LOG(ERROR) << __func__ << " - Specification is empty";
    return false;
  }

  for (const Specification& spec : specs) {
    const std::string module_name = spec.module_name();
    base::TaskRunner* task_runner =
      CreateTaskRunnerForModule(module_name);
    if (!task_runner) {
      LOG(WARN) << " Can not create task runner for module. "
                << module_name;
      continue;
    }

    if (!CreateModuleExecutor(module_name, task_runner)) {
      continue;
    }

    ModuleClient* module_client =
      dynamic_cast<ModuleClient*>(executors_.at(module_name).get());
    if (!loader_.LoadModule(module_client, spec)) {
      return false;
    }
  }
  return true;
}

void ModuleController::RunningModule() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  if (module_names.empty()) {
    LOG(WARN) << " Empty module on loader";
    return;
  }

  for (const std::string& name : module_names) {
    LinkModule* module = loader_.GetModule(name);
    executors_[name]->RunningModule(module);
  }
}

void ModuleController::Destroy() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  for (const std::string& name : module_names) {
    loader_.UnLoadModule(name);
  }
}

void ModuleController::TerminateModule(const std::string& module_name) {
  LOG(TRACE) << __func__;
  base::TaskDispatcher* task_dispatcher = task_manager_->GetTaskDispatcher();

  task_dispatcher->PostTask(module_name,
    base::Bind(&ModuleLoader::UnLoadModule, &loader_, module_name));

  task_dispatcher->PostTask(module_name,
    base::Bind(&base::TaskManager::StopRunner, task_manager_, module_name));
}

base::TaskRunner* ModuleController::CreateTaskRunnerForModule(
  const std::string& module_name) {
  return task_manager_->CreateTaskRunner(
    module_name, base::TaskRunner::Type::SEQUENCE);
}

bool ModuleController::CreateModuleExecutor(
  const std::string& module_name, base::TaskRunner* task_runner) {
  if (executors_.find(module_name) != executors_.end()) {
    LOG(WARN) << " ModuleExecutor already exist. " << module_name;
    return false;
  }

  std::unique_ptr<ModuleExecutor> executor(
    new ModuleExecutor(task_runner, this));
  executors_.insert({module_name, std::move(executor)});
  return true;
}

}  // namespace module
}  // namespace link
