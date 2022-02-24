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

namespace link {
namespace module {

UserModule::UserModule(
  const std::string& module_name, UserModuleBase::ModuleClient* client)
  : UserModuleBase(module_name, client) {
}

UserModule::~UserModule() {
}

void UserModule::Initialize(const base::Json& arguments) {
  arguments_ = arguments;
  Init();
}

void UserModule::Process() {
  Run();
}

void UserModule::Terminate() {
  Shutdown();
}

}  // namespace module
}  // namespace link
