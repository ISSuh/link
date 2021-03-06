/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/base/user_module_base.h"

namespace nlink {
namespace module {

UserModuleBase::UserModuleBase(
  const std::string& module_name, ModuleClient* client)
  : module_name_(module_name),
    running_state_(false),
    client_(client) {
}

UserModuleBase::~UserModuleBase() {
}

bool UserModuleBase::IsRunning() const {
  return running_state_;
}

}  // namespace module
}  // namespace nlink

