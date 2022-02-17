/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_register.h"

namespace link {
namespace module {

ModuleRegister::ModuleFactoryMap ModuleRegister::factories_
  = ModuleFactoryMap();

void ModuleRegister::ReleaseModuleFactory(const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return;
  }

  --factories_[class_name].ref_count;
  if (factories_[class_name].ref_count <= 0) {
    factories_[class_name].factory.reset();
    factories_.erase(class_name);
  }
}

}  // namespace module
}  // namespace link
