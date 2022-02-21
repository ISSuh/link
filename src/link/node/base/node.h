/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NODE_BASE_NODE_H_
#define LINK_NODE_BASE_NODE_H_

#include <memory>

#include "link/module/controller/controller.h"
#include "link/base/task_manager.h"
#include "link/node/base/arguments.h"
#include "link/base/macro.h"

namespace link {
namespace node {

class LinkNode {
 public:
  LinkNode();
  ~LinkNode();

  bool Init(int32_t argc, char *argv[]);
  void Run();

 private:
  base::TaskManager task_manager_;
  std::unique_ptr<module::ModuleController> controller_;
  std::unique_ptr<Arguments> args_;

  DISAALOW_COPY_AND_ASSIGN(LinkNode)
};

}  // namespace node
}  // namespace link

#endif  // LINK_NODE_BASE_NODE_H_
