/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_LOADER_MODULE_LOADER_UTIL_H_
#define LINK_LAUNCHER_LOADER_MODULE_LOADER_UTIL_H_

#include <string>
#include <map>

#include "link/launcher/loader/module_factory.h"

namespace link {
namespace launcher {

class ModuleRegister {
 public:
  template<typename UserModule, typename UserModuleBase>
  static void CreateModuleFactory(
    const std::string& class_name, const std::string& base_class_name);

  static void ReleaseModuleFactory(const std::string& class_name);

  template <typename UserModuleBase>
  static const AbstractModlueFactory<UserModuleBase>* GetModuleFactory(
    const std::string& class_name);

 private:
  using ModuleFactoryMap = std::map<std::string, AbstractModuleFactoryBase*>;
  static ModuleFactoryMap factories_;
};

template<typename UserModule, typename UserModuleBase>
void ModuleRegister::CreateModuleFactory(const std::string& class_ame,
                                        const std::string& base_class_name) {
  AbstractModlueFactory<UserModuleBase>* moduleFactrory =
      new ModuleFactory<UserModule, UserModuleBase>(
        class_name, base_class_name);

  factories_[class_name] = moduleFactrory;
}

template <typename UserModuleBase>
const AbstractModlueFactory<UserModuleBase>* ModuleRegister::GetModuleFactory(
  const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return nullptr;
  }
  return dynamic_cast<AbstractModlueFactory<UserModuleBase>*>(
    factories_[class_name]);
}

void ModuleRegister::ReleaseModuleFactory(const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return;
  }

  delete factories_[class_name];
  factories_[class_name] = nullptr;

  factories_.erase(class_name);
}

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_LOADER_MODULE_LOADER_UTIL_H_
