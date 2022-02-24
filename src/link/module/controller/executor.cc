/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/executor.h"

namespace link {
namespace module {

ModuleExecutor::ModuleExecutor(base::TaskRunner* task_runner)
  : task_runner_(task_runner),
    is_running_(false) {
}

ModuleExecutor::~ModuleExecutor() {
}

void ModuleExecutor::RunningModule(LinkModule* module) {
  is_running_ = true;

  // task_runner_->PostTask(base::Bind(&LinkModule::Initialize, module));
  // task_runner_->PostTask(base::Bind(&LinkModule::Process, module));
  // task_runner_->PostTask(base::Bind(&LinkModule::Terminate, module));

  module->Initialize();
  module->Process();
  module->Terminate();
}

void ModuleExecutor::OnTerminate() {
  
}

}  // namespace module
}  // namespace link
