/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/base/user_module.h"

#include <string>
#include <memory>
#include <algorithm>

#include "link/module/base/user_module_base.h"
#include "link/module/loader/module_register_helper.h"
#include "link/base/logging.h"

namespace nlink {
namespace module {

UserModule::UserModule(
  const std::string& module_name, ModuleClient* client)
  : UserModuleBase(module_name, client),
    logger_(nullptr) {
  LOG(INFO) << __func__;
}

UserModule::~UserModule() {
}

base::TaskRunner* UserModule::GetTaskRunner() const {
  std::shared_ptr<base::TaskRunner> task_runner = task_runner_weak_.lock();
  if (nullptr == task_runner) {
    return nullptr;
  }
  return task_runner.get();
}

void UserModule::Initialize(
  std::weak_ptr<base::TaskRunner> task_runner_weak,
  const base::Json& arguments) {
  logger_ = base::LoggerManager::Instance()->CreateLogger(module_name_);
  task_runner_weak_ = task_runner_weak;

  arguments_ = arguments;
  running_state_ = true;
  Init();

  client_->AfterInitialize(module_name_);
}

void UserModule::Process() {
  Run();

  client_->AfterProcess(module_name_);
}

void UserModule::Terminate() {
  Shutdown();
  running_state_ = false;

  client_->AfterTerminate(module_name_);
}

}  // namespace module
}  // namespace nlink
