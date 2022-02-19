/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_EXECUTOR_H_
#define LINK_MODULE_CONTROLLER_EXECUTOR_H_

#include <string>
#include <map>
#include <memory>


#include "link/module/loader/module_loader.h"
#include "link/base/task/task_runner.h"
#include "link/base/macro.h"

namespace link {
namespace module {

class ModuleExecutor {
 public:
  ModuleExecutor();
  ~ModuleExecutor();

  

 private:
  ModuleLoader loader_;
  base::TaskRunner* task_runner_;

  DISAALOW_COPY_AND_ASSIGN(ModuleExecutor)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_EXECUTOR_H_
