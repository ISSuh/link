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
    task_manager_(std::make_shared<base::TaskManager>()),
    logger_(nullptr) {
}

LinkNode::~LinkNode() {
}

bool LinkNode::Init(int32_t argc, char *argv[]) {
  base::LoggerManager::Instance()->SetTaskManager(task_manager_);

  if (!args_.ParseFromeProcessArguments(argc, argv)) {
    return false;
  }

  if (!CreateModuleController()) {
    return false;
  }
  return true;
}

void LinkNode::Run() {
  controller_->LoadingModule(
    args_.Modulespecifications(),
    [this](bool status) {
      this->ModuleLoadedStatus(status);
    });

  task_manager_->WaitForTerminateAllTaskRunner();
}

bool LinkNode::CreateModuleController() {
  controller_.reset(new module::ModuleController(task_manager_));
  return true;
}

void LinkNode::ModuleLoadedStatus(bool status) {
  if (!status) {
    controller_->Destroy();
  }
  controller_->RunningModule(
      [this](bool status) {
        this->ModuleRunningStatus(status);
      });
}

void LinkNode::ModuleRunningStatus(bool status) {
  if (!status) {
    controller_->Destroy();
  }
}


}  // namespace node
}  // namespace nlink
