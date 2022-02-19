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
    runner_({nullptr, nullptr}) {
}

ModuleController::~ModuleController() {
}

void ModuleController::CreateRunner() {
  std::unique_ptr<ModuleLoader> loader(new ModuleLoader());
  std::unique_ptr<ModuleExecutor> executor(new ModuleExecutor(task_runner_));
  runner_.loader = std::move(loader);
  runner_.executor = std::move(executor);
}

bool ModuleController::LoadingModule(const std::vector<Specification>& specs) {
  if (specs.empty()) {
    LOG(ERROR) << __func__ << " - Specification is empty";
    return false;
  }

  for (const Specification& spec : specs) {
    if (runner_.loader->LoadModule(spec)) {
      return false;
    }
  }
  return true;
}

void ModuleController::Destroy() {
  std::vector<std::string > module_names = runner_.loader->ModuleNames();
  for (const std::string& name : module_names) {
    runner_.loader->UnLoadModule(name);
  }
}

}  // namespace module
}  // namespace link
