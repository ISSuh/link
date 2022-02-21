/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_EXECUTOR_H_
#define LINK_MODULE_CONTROLLER_EXECUTOR_H_

#include "link/module/base/module.h"
#include "link/base/task/task_runner.h"
#include "link/base/macro.h"

namespace link {
namespace module {


class ModuleExecutor {
 public:
  explicit ModuleExecutor(base::TaskRunner* task_runner);
  ~ModuleExecutor();

  void RunningModule(Module* module);

 private:
  base::TaskRunner* task_runner_;
  bool is_running_;

  DISAALOW_COPY_AND_ASSIGN(ModuleExecutor)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_EXECUTOR_H_
