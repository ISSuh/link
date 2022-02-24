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

ModuleController::ModuleController(base::TaskRunner* task_runner)
  : task_runner_(task_runner),
    executor_(task_runner_, this) {
}

ModuleController::~ModuleController() {
}

bool ModuleController::LoadingModule(const std::vector<Specification>& specs) {
  if (specs.empty()) {
    LOG(ERROR) << __func__ << " - Specification is empty";
    return false;
  }

  for (const Specification& spec : specs) {
    ModuleClient* module_client =
      dynamic_cast<ModuleClient*>(&executor_);
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
    executor_.RunningModule(module);
  }
}

void ModuleController::Destroy() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  for (const std::string& name : module_names) {
    loader_.UnLoadModule(name);
  }
}

void ModuleController::TerminateModule(const std::string& module_name) {
  LOG(INFO) << __func__;
  loader_.UnLoadModule(module_name);
}

}  // namespace module
}  // namespace link
