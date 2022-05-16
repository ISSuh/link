/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_EXECUTOR_H_
#define LINK_MODULE_CONTROLLER_EXECUTOR_H_

#include <string>

#include "link/module/base/module.h"
#include "link/module/base/user_module_base.h"
#include "link/base/task/task_runner.h"
#include "link/base/macro.h"

namespace nlink {
namespace module {

class ModuleExecutor
  : public ModuleClient {
 public:
  class ModuleExecutorClient {
   public:
    virtual void TerminateModule(const std::string& module_name) = 0;
  };

  explicit ModuleExecutor(
    base::TaskRunner* task_runner,
    ModuleExecutorClient* client);
  ~ModuleExecutor();

  void RunningModule(LinkModule* module);

  // ModuleClient
  void AfterInitialize(const std::string& module_name) override;
  void AfterProcess(const std::string& module_name) override;
  void AfterTerminate(const std::string& module_name) override;

 private:
  base::TaskRunner* task_runner_;
  ModuleExecutorClient* client_;

  LinkModule* module_;

  DISAALOW_COPY_AND_ASSIGN(ModuleExecutor)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_CONTROLLER_EXECUTOR_H_
