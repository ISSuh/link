/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/executor.h"

namespace nlink {
namespace module {

ModuleExecutor::ModuleExecutor(
  base::TaskRunner* task_runner, ModuleExecutorClient* excutor_client)
  : task_runner_(task_runner),
    excutor_client_(excutor_client),
    module_(nullptr) {
}

ModuleExecutor::~ModuleExecutor() = default;

void ModuleExecutor::RunningModule(LinkModule* module) {
  module_ = module;
  task_runner_->PostTask(
    [this, module = module_]() { module->Initialize(); });
  // module_->Initialize();
}

void ModuleExecutor::AfterInitialize(const std::string& module_name) {
  task_runner_->PostTask(
    [this, module = module_]() { module->Process(); });

  // module_->Process();
}

void ModuleExecutor::AfterProcess(const std::string& module_name) {
  task_runner_->PostTask(
    [this, module = module_]() { module->Terminate(); });

  // module_->Terminate();
}

void ModuleExecutor::AfterTerminate(const std::string& module_name) {
  task_runner_->PostTask(
    [this, excutor_client = excutor_client_, module_name]() {
      excutor_client->TerminateModule(module_name);
    });

  // excutor_client_->TerminateModule(module_name);
}

}  // namespace module
}  // namespace nlink
