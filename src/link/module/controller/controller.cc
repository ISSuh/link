/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/controller.h"

#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace module {

const char* kModuleTaskRunnerName = "ModuleTaskRunner";
const char* kControllerTaskRunnerNamePostfix = "_Contoller";

ModuleController::ModuleController(
  std::shared_ptr<base::TaskManager> task_manager)
  : task_manager_(task_manager),
    loaded_module_count_(0) {
}

ModuleController::~ModuleController() {
}

void ModuleController::LoadingModule(
  const std::vector<Specification>& specs, StatusCallback status_callback) {
  if (specs.empty()) {
    LOG(ERROR) << __func__ << " - Specification is empty";
    status_callback(false);
    return;
  }

  size_t will_loaded_module_count = loaded_module_count_.load() + specs.size();

  for (const Specification& spec : specs) {
    const std::string module_name = spec.module_name();
    const std::string controller_task_runner_name =
      module_name + kControllerTaskRunnerNamePostfix;

    base::TaskRunner* controller_task_runner =
      task_manager_->CreateTaskRunner(
        controller_task_runner_name,
        base::TaskRunner::Type::CONCURRENT, specs.size());

    controller_task_runners_.insert({module_name, controller_task_runner});

    controller_task_runners_[module_name]->PostTask(
      [this, &spec, &will_loaded_module_count, status_callback]() {
        this->LodingModuleInternal(
          spec, will_loaded_module_count, status_callback);
      });
  }
}

void ModuleController::RunningModule(StatusCallback status_callback) {
  std::vector<std::string > module_names = loader_.ModuleNames();
  if (module_names.empty()) {
    LOG(WARNING) << " Empty module on loader";
    status_callback(false);
    return;
  }

  for (const std::string& name : module_names) {
    LinkModule* module = loader_.GetModule(name);
    ModuleExecutor* excutor =  executors_[name].get();

    controller_task_runners_[name]->PostTask(
      [&excutor, &module]() {
        excutor->RunningModule(module);
      });
  }

  status_callback(true);
}

void ModuleController::Destroy() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  for (const std::string& name : module_names) {
    TerminateModule(name);
    loader_.UnLoadModule(name);
  }
}

void ModuleController::TerminateModule(const std::string& module_name) {
  LOG(TRACE) << __func__;
  const std::string controller_task_runner_name =
    module_name + kControllerTaskRunnerNamePostfix;

  TerminateModuleTaskRunner(module_name);

  if (task_manager_->CheckRunningByLabel(controller_task_runner_name)) {
    controller_task_runners_[module_name]->PostTask(
      [this, &module_name]() {
        this->TerminateModuleInternal(module_name);
      });
  }
}

void ModuleController::LodingModuleInternal(
  const Specification spec,
  size_t will_loaded_module_count,
  StatusCallback status_callback) {
  const std::string module_name = spec.module_name();

  base::TaskRunner* task_runner =
    CreateTaskRunnerForModule(module_name);
  if (!task_runner) {
    LOG(WARNING) << " Can not create task runner for module. " << module_name;
    status_callback(false);
    return;
  }

  if (!CreateModuleExecutor(module_name, task_runner)) {
    status_callback(false);
    TerminateModule(module_name);
    return;
  }

  ModuleClient* module_client =
    dynamic_cast<ModuleClient*>(executors_.at(module_name).get());
  if (!loader_.LoadModule(task_runner, module_client, spec)) {
    status_callback(false);
    TerminateModule(module_name);
    return;
  }

  loaded_module_count_.fetch_add(1);
  if (will_loaded_module_count == loaded_module_count_.load()) {
    status_callback(true);
  }
}

void ModuleController::TerminateModuleInternal(const std::string& module_name) {
  const std::string controller_task_runner_name =
    module_name + kControllerTaskRunnerNamePostfix;

  if (loader_.HasModule(module_name)) {
    loader_.UnLoadModule(module_name);
  }

  TerminateModuleTaskRunner(controller_task_runner_name);
}

base::TaskRunner* ModuleController::CreateTaskRunnerForModule(
  const std::string& module_name) {
  return task_manager_->CreateTaskRunner(
    module_name, base::TaskRunner::Type::SEQUENCE);
}

bool ModuleController::CreateModuleExecutor(
  const std::string& module_name, base::TaskRunner* task_runner) {
  if (executors_.find(module_name) != executors_.end()) {
    LOG(WARNING) << " ModuleExecutor already exist. " << module_name;
    return false;
  }

  std::unique_ptr<ModuleExecutor> executor(
    new ModuleExecutor(task_runner, this));
  executors_.insert({module_name, std::move(executor)});
  return true;
}

void ModuleController::TerminateModuleTaskRunner(
  const std::string& task_runner_label) {
  if (task_manager_->CheckRunningByLabel(task_runner_label)) {
    base::TaskDispatcher* task_dispatcher = task_manager_->GetTaskDispatcher();

    auto StopRunnerCallback =
      [task_manager = task_manager_, &task_runner_label]() {
        task_manager->StopRunner(task_runner_label);
      };

    task_dispatcher->PostTask(task_runner_label, std::move(StopRunnerCallback));
  }
}

}  // namespace module
}  // namespace nlink
