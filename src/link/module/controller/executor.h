/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_EXECUTOR_H_
#define LINK_MODULE_CONTROLLER_EXECUTOR_H_

#include "link/module/base/module.h"
#include "link/module/base/user_module_base.h"
#include "link/base/task/task_runner.h"
#include "link/base/macro.h"

namespace link {
namespace module {

class ModuleExecutor : public UserModuleBase::ModuleClient {
 public:
  explicit ModuleExecutor(base::TaskRunner* task_runner);
  ~ModuleExecutor();

  void RunningModule(LinkModule* module);

  // ModuleClient
  void OnTerminate() override;

 private:
  base::TaskRunner* task_runner_;
  bool is_running_;

  DISAALOW_COPY_AND_ASSIGN(ModuleExecutor)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_EXECUTOR_H_
