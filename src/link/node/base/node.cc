/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/node.h"

namespace nlink {
namespace node {

LinkNode::LinkNode()
  : controller_(nullptr),
    task_manager_(std::make_shared<base::TaskManager>()) {
}

LinkNode::~LinkNode() {
}

bool LinkNode::Init(int32_t argc, char *argv[]) {
  if (!args_.ParseFromeProcessArguments(argc, argv)) {
    return false;
  }

  if (!CreateModuleControllerAndLoadModules()) {
    return false;
  }
  return true;
}

void LinkNode::Run() {
  controller_->RunningModule();
  task_manager_->WaitForTerminateAllTaskRunner();
}

bool LinkNode::CreateModuleControllerAndLoadModules() {
  controller_.reset(new module::ModuleController(task_manager_));

  if (!controller_->LoadingModule(args_.module_specs())) {
    return false;
  }

  return true;
}

}  // namespace node
}  // namespace nlink
