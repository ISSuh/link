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

#include "link/module/controller/executor.h"
#include "link/module/base/specification.h"
#include "link/base/task_manager.h"
#include "link/base/macro.h"

namespace link {
namespace module {

class ModuleController {
 public:
  ModuleController();
  ~ModuleController();

  void WaitForAllModuleTerminate() const;

 private:
  void CreateModuleExecutor(const std::string& node_name);

  std::unique_ptr<base::TaskManager> task_manager_;
  std::map<std::string, std::unique_ptr<ModuleExecutor>> executors_;

  DISAALOW_COPY_AND_ASSIGN(ModuleController)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_CONTROLLER_H_
