/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NODE_BASE_NODE_H_
#define LINK_NODE_BASE_NODE_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/logger.h"
#include "link/base/task_manager.h"
#include "link/module/controller/controller.h"
#include "link/node/base/arguments.h"

namespace nlink {
namespace node {

class LinkNode {
 public:
  LinkNode();
  ~LinkNode();

  bool Init(int32_t argc, char *argv[]);
  void Run();

 private:
  bool CreateModuleController();
  void ModuleLoadedStatus(bool status);
  void ModuleRunningStatus(bool status);

  std::unique_ptr<module::ModuleController> controller_;
  std::shared_ptr<base::TaskManager> task_manager_;
  std::shared_ptr<base::Logger> logger_;

  Arguments args_;

  DISAALOW_COPY_AND_ASSIGN(LinkNode)
};

}  // namespace node
}  // namespace nlink

#endif  // LINK_NODE_BASE_NODE_H_
