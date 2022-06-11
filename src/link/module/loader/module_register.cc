/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_register.h"

namespace nlink {
namespace module {

std::atomic<ModuleRegister*> ModuleRegister::instance_{nullptr};
std::mutex ModuleRegister::mutex_;

ModuleRegister* ModuleRegister::GetInstance() {
  if (nullptr == instance_) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr == instance_) {
      instance_ = new ModuleRegister();
    }
  }
  return instance_;
}

void ModuleRegister::ReleaseModuleFactory(const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return;
  }


  // TODO(issuh) : should change release factory logic
  --factories_[class_name].ref_count;
  if (factories_[class_name].ref_count <= 0) {
    // delete factories_[class_name].factory;
    factories_.erase(class_name);
  }
}

}  // namespace module
}  // namespace nlink
