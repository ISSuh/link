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

ModuleController::~ModuleController() = default;

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

    std::weak_ptr<base::TaskRunner> controller_task_runner_weak =
      task_manager_->CreateTaskRunner(
        module_name, controller_task_runner_name,
        base::TaskRunner::Type::SEQUENCE, specs.size());

    controller_task_runners_.insert({module_name, controller_task_runner_weak});

    std::shared_ptr<base::TaskRunner> controller_task_runner =
      controller_task_runner_weak.lock();

    if (nullptr == controller_task_runner) {
      return;
    }

    controller_task_runner->PostTask(
      [this, spec, will_loaded_module_count,
       status_callback, controller_task_runner_weak]() {
        this->LodingModuleInternal(
          controller_task_runner_weak, spec,
          will_loaded_module_count, status_callback);
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

    std::weak_ptr<base::TaskRunner> task_runner_weak =
      controller_task_runners_[name];

    std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak.lock();
    if (nullptr == task_runner) {
      continue;
    }

    task_runner->PostTask(
      [excutor, module]() {
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

  TerminateModuleTaskRunner(
    module_name,
    module_name,
    [this, module_name]() {
      this->TerminateModuleInternal(module_name);
    });

  // if (task_manager_->IsTaskRunnerRunning(
  //       module_name, controller_task_runner_name)) {
  //   controller_task_runners_[module_name]->PostTask(
  //     [this, module_name]() {
  //       this->TerminateModuleInternal(module_name);
  //     });
  // }
}

void ModuleController::LodingModuleInternal(
  std::weak_ptr<base::TaskRunner> controller_task_runner_weak,
  const Specification spec,
  size_t will_loaded_module_count,
  StatusCallback status_callback) {
  const std::string module_name = spec.module_name();

  if (!CreateModuleExecutor(module_name, controller_task_runner_weak)) {
    status_callback(false);
    TerminateModule(module_name);
    return;
  }

  std::weak_ptr<base::TaskRunner> task_runner_weak =
    CreateWorkerTaskRunnerForModule(module_name, module_name);

  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak.lock();
  if (nullptr == task_runner) {
    LOG(WARNING) << " Can not create task runner for module. " << module_name;
    status_callback(false);
    return;
  }

  ModuleClient* module_client =
    dynamic_cast<ModuleClient*>(executors_.at(module_name).get());
  if (!loader_.LoadModule(task_runner_weak, module_client, spec)) {
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

  TerminateModuleTaskRunner(
    module_name,
    controller_task_runner_name,
    [this, module_name]() {
      this->UnLoadModule(module_name);
    });
}

std::weak_ptr<base::TaskRunner>
ModuleController::CreateWorkerTaskRunnerForModule(
  const std::string& module_name, const std::string& task_runner_label) {
  return task_manager_->CreateTaskRunner(
    module_name, task_runner_label, base::TaskRunner::Type::SEQUENCE);
}

bool ModuleController::CreateModuleExecutor(
  const std::string& module_name,
  std::weak_ptr<base::TaskRunner> task_runner_weak) {
  if (executors_.find(module_name) != executors_.end()) {
    LOG(WARNING) << " ModuleExecutor already exist. " << module_name;
    return false;
  }

  ModuleExecutor::TerminateModuleCallback callback =
    [this](const std::string& module_name) {
        this->TerminateModule(module_name);
    };

  std::unique_ptr<ModuleExecutor> executor =
    std::make_unique<ModuleExecutor>(task_runner_weak, std::move(callback));
  executors_.insert({module_name, std::move(executor)});
  return true;
}

void ModuleController::TerminateModuleTaskRunner(
  const std::string& task_runner_group_label,
  const std::string& task_runner_label,
  std::function<void()> after_terminate_task_runner) {
  if (!task_manager_->IsTaskRunnerRunning(
    task_runner_group_label, task_runner_label)) {
    return;
  }

  base::TaskDispatcher* task_dispatcher = task_manager_->GetTaskDispatcher();

  auto stop_runner_task =
    [task_manager = task_manager_,
     task_runner_group_label,
     task_runner_label,
     after_terminate_task_runner]() {
      task_manager->StopRunner(task_runner_group_label, task_runner_label);
      after_terminate_task_runner();
    };

  task_dispatcher->PostTask(
    task_runner_group_label, task_runner_label, std::move(stop_runner_task));
}

void ModuleController::UnLoadModule(const std::string& module_name) {
  if (loader_.HasModule(module_name)) {
    loader_.UnLoadModule(module_name);
  }
}

}  // namespace module
}  // namespace nlink
