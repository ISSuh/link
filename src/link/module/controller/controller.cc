/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/controller/controller.h"

namespace link {
namespace module {

const char* kModuleTaskRunnerName = "ModuleTaskRunner";

ModuleController::ModuleController()
  : task_manager_(new base::TaskManager()) {
  task_manager_->CreateTaskRunner(
    kModuleTaskRunnerName, base::TaskRunner::Type::CONCURRENT);
}

ModuleController::~ModuleController() {
}

void ModuleController::WaitForAllModuleTerminate() const {
  task_manager_->WaitForTerminateAllTaskRunner();
}



}  // namespace module
}  // namespace link
