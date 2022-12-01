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

#include "link/base/macro.h"
#include "link/base/callback.h"
#include "link/base/task_manager.h"
#include "link/module/loader/module_loader.h"
#include "link/module/controller/executor.h"
#include "link/module/base/specification.h"

namespace nlink {
namespace module {

class ModuleController {
 public:
  using StatusCallback = std::function<void(bool)>;

  explicit ModuleController(std::shared_ptr<base::TaskManager> task_manager);
  virtual ~ModuleController();

  void LoadingModule(
    const std::vector<Specification>& specs, StatusCallback status_callback);

  void RunningModule(StatusCallback status_callback);

  void Destroy();

  void TerminateModule(const std::string& module_name);

 private:
  void LodingModuleInternal(
    std::weak_ptr<base::TaskRunner> controller_task_runner,
    const Specification spec,
    size_t will_loaded_module_count,
    StatusCallback status_callback);

  void TerminateModuleInternal(const std::string& module_name);

  std::weak_ptr<base::TaskRunner> CreateWorkerTaskRunnerForModule(
    const std::string& module_name, const std::string& task_runner_label);

  bool CreateModuleExecutor(
    const std::string& module_name,
    std::weak_ptr<base::TaskRunner> task_runner);

  void TerminateModuleTaskRunner(
    const std::string& task_runner_group_label,
    const std::string& task_runner_label,
    std::function<void()> after_terminate_task_runner);

  void UnLoadModule(const std::string& module_name);

  std::shared_ptr<base::TaskManager> task_manager_;

  ModuleLoader loader_;

  using ModuleExcutorMap =
    std::map<std::string, std::unique_ptr<ModuleExecutor>>;
  ModuleExcutorMap executors_;

  using ControllerTaskRunnerMap =
    std::map<std::string, std::weak_ptr<base::TaskRunner>>;
  ControllerTaskRunnerMap controller_task_runners_;

  std::atomic_size_t loaded_module_count_;

  DISAALOW_COPY_AND_ASSIGN(ModuleController)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_CONTROLLER_CONTROLLER_H_
