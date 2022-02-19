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

void ModuleExecutor::RunningModule(Module* module) {
  is_running_ = true;

  task_runner_->PostTask(base::Bind(Module::Initialize, module));
  task_runner_->PostTask(base::Bind(Module::Process, module));
}

ModuleExecutor::~ModuleExecutor() {
}

}  // namespace module
}  // namespace link
