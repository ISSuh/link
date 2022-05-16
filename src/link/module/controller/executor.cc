/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/executor.h"

namespace nlink {
namespace module {

ModuleExecutor::ModuleExecutor(
  base::TaskRunner* task_runner, ModuleExecutorClient* client)
  : task_runner_(task_runner),
    client_(client),
    module_(nullptr) {
}

ModuleExecutor::~ModuleExecutor() {
}

void ModuleExecutor::RunningModule(LinkModule* module) {
  module_ = module;
  task_runner_->PostTask(base::Bind(&LinkModule::Initialize, module));
}

void ModuleExecutor::AfterInitialize(const std::string& module_name) {
  task_runner_->PostTask(base::Bind(&LinkModule::Process, module_));
}

void ModuleExecutor::AfterProcess(const std::string& module_name) {
  task_runner_->PostTask(base::Bind(&LinkModule::Terminate, module_));
}

void ModuleExecutor::AfterTerminate(const std::string& module_name) {
  client_->TerminateModule(module_name);
}

}  // namespace module
}  // namespace nlink
