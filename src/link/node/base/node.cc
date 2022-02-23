/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/node.h"

namespace link {
namespace node {

LinkNode::LinkNode()
  : controller_(nullptr) {
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
  task_manager_.WaitForTerminateAllTaskRunner();
}

bool LinkNode::CreateModuleControllerAndLoadModules() {
  base::TaskRunner* task_runner = task_manager_.CreateTaskRunner(
    args_.node_name(), base::TaskRunner::Type::SEQUENCE);
  if (!task_runner) {
    return false;
  }

  controller_.reset(new module::ModuleController(task_runner));

  if (!controller_->LoadingModule(args_.module_specs())) {
    return false;
  }

  return true;
}

}  // namespace node
}  // namespace link
