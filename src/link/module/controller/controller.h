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
#include <atomic>
#include <functional>

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
  using StatusCallback = std::function<void(bool)>;

  explicit ModuleController(std::shared_ptr<base::TaskManager> task_manager_);
  virtual ~ModuleController();

  void LoadingModule(
    const std::vector<Specification>& specs, StatusCallback status_callback);
  void RunningModule(StatusCallback status_callback);
  void Destroy();

  // ModuleExecutorClient
  void TerminateModule(const std::string& module_name) override;

 private:
  void LodingModuleInternal(
    base::TaskRunner* controller_task_runner,
    const Specification spec,
    size_t will_loaded_module_count,
    StatusCallback status_callback);
  void TerminateModuleInternal(const std::string& module_name);

  base::TaskRunner* CreateTaskRunnerForModule(const std::string& module_name);
  bool CreateModuleExecutor(
    const std::string& module_name, base::TaskRunner* task_runner);

  void TerminateModuleTaskRunner(
    const std::string& task_runner_label);

  std::shared_ptr<base::TaskManager> task_manager_;

  ModuleLoader loader_;
  std::map<std::string, std::unique_ptr<ModuleExecutor>> executors_;
  std::map<std::string, base::TaskRunner*> controller_task_runners_;

  std::atomic_size_t loaded_module_count_;

  DISAALOW_COPY_AND_ASSIGN(ModuleController)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_CONTROLLER_CONTROLLER_H_
