/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_CONTROLLER_H_
#define LINK_MODULE_CONTROLLER_CONTROLLER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "link/module/loader/module_loader.h"
#include "link/module/controller/executor.h"
#include "link/module/base/specification.h"
#include "link/base/task_manager.h"
#include "link/base/macro.h"

namespace nlink {
namespace module {

class ModuleController
  : public ModuleExecutor::ModuleExecutorClient {
 public:
  explicit ModuleController(base::TaskManager* task_manager);
  ~ModuleController();

  bool LoadingModule(const std::vector<Specification>& specs);
  void RunningModule();
  void Destroy();

  // ModuleExecutorClient
  void TerminateModule(const std::string& module_name);

 private:
  base::TaskRunner* CreateTaskRunnerForModule(const std::string& module_name);
  bool CreateModuleExecutor(
    const std::string& module_name, base::TaskRunner* task_runner);

  base::TaskManager* task_manager_;
  ModuleLoader loader_;
  std::map<std::string, std::unique_ptr<ModuleExecutor>> executors_;

  DISAALOW_COPY_AND_ASSIGN(ModuleController)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_CONTROLLER_CONTROLLER_H_
