/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_register.h"

namespace link {
namespace module {

std::atomic<ModuleRegister*> ModuleRegister::instance_;
std::mutex ModuleRegister::lock_;

ModuleRegister* ModuleRegister::GetInstance() {
  ModuleRegister* instance = instance_.load(std::memory_order_acquire);
  if (!instance) {
      std::lock_guard<std::mutex> myLock(lock_);
      instance = instance_.load(std::memory_order_relaxed);
      if (!instance) {
        instance = new ModuleRegister();
        instance_.store(instance, std::memory_order_release);
      }
  }
  return instance;
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
}  // namespace link
