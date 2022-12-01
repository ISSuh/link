/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_EXECUTOR_H_
#define LINK_MODULE_CONTROLLER_EXECUTOR_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/callback.h"
#include "link/base/task/task_runner.h"
#include "link/module/base/module.h"
#include "link/module/base/user_module_base.h"

namespace nlink {
namespace module {

class ModuleExecutor : public ModuleClient {
 public:
  using TerminateModuleCallback = base::Callback<void(const std::string&)>;

  ModuleExecutor(
    std::weak_ptr<base::TaskRunner> task_runner,
    TerminateModuleCallback terminate_module_callback);
  virtual ~ModuleExecutor();

  void RunningModule(LinkModule* module);

  // ModuleClient
  void AfterInitialize(const std::string& module_name) override;
  void AfterProcess(const std::string& module_name) override;
  void AfterTerminate(const std::string& module_name) override;

 private:
  std::weak_ptr<base::TaskRunner> task_runner_weak_;

  LinkModule* module_;
  TerminateModuleCallback terminate_module_callback_;

  DISAALOW_COPY_AND_ASSIGN(ModuleExecutor)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_CONTROLLER_EXECUTOR_H_
