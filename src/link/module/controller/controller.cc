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
    executor_(task_runner_) {
}

ModuleController::~ModuleController() {
}

bool ModuleController::LoadingModule(const std::vector<Specification>& specs) {
  if (specs.empty()) {
    LOG(ERROR) << __func__ << " - Specification is empty";
    return false;
  }

  for (const Specification& spec : specs) {
    if (loader_.LoadModule(spec)) {
      return false;
    }
  }
  return true;
}

void ModuleController::RunningModule() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  for (const std::string& name : module_names) {
    Module* module = loader_.GetModule(name);
    executor_.RunningModule(module);
  }
}

void ModuleController::Destroy() {
  std::vector<std::string > module_names = loader_.ModuleNames();
  for (const std::string& name : module_names) {
    loader_.UnLoadModule(name);
  }
}

}  // namespace module
}  // namespace link
