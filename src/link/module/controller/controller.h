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

namespace link {
namespace module {

class ModuleController {
 public:
  explicit ModuleController(base::TaskRunner* task_runner);
  ~ModuleController();

  void CreateRunner();
  bool LoadingModule(const std::vector<Specification>& specs);
  void RunningModule();
  void Destroy();


 private:
  struct Runner {
    std::unique_ptr<ModuleLoader> loader;
    std::unique_ptr<ModuleExecutor> executor;
  };

  base::TaskRunner* task_runner_;
  Runner runner_;

  DISAALOW_COPY_AND_ASSIGN(ModuleController)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_CONTROLLER_H_
