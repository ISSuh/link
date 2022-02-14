/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULE_LOADER_UTIL_H_
#define LINK_MODULE_LOADER_MODULE_LOADER_UTIL_H_

#include <string>
#include <map>
#include <memory>
#include <utility>

#include "link/module/loader/module_factory.h"

namespace link {
namespace module {

class ModuleRegister {
 public:
  struct FactoryWrapper {
    uint32_t ref_count = 0;
    std::unique_ptr<AbstractModuleFactoryBase> factory = nullptr;
  };

  template<typename UserModule, typename UserModuleBase>
  static void CreateModuleFactory(
    const std::string& class_name, const std::string& base_class_name);

  static void ReleaseModuleFactory(const std::string& class_name);

  template <typename UserModuleBase>
  static const AbstractModlueFactory<UserModuleBase>* GetModuleFactory(
    const std::string& class_name);

 private:
  using ModuleFactoryMap = std::map<std::string, FactoryWrapper>;
  static ModuleFactoryMap factories_;
};

template<typename UserModule, typename UserModuleBase>
void ModuleRegister::CreateModuleFactory(const std::string& class_ame,
                                        const std::string& base_class_name) {
  if (factories_.find(class_ame) != factories_.end()) {
    ++factories_[class_name].ref_count;
    return;
  }

  std::unique_ptr<AbstractModuleFactoryBase> factory(
    new ModuleFactory<UserModule, UserModuleBase>(
        class_name, base_class_name));

  factories_[class_name] = {1, std::move(factory)};
}

template <typename UserModuleBase>
const AbstractModlueFactory<UserModuleBase>* ModuleRegister::GetModuleFactory(
  const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return nullptr;
  }
  return dynamic_cast<AbstractModlueFactory<UserModuleBase>*>(
    factories_[class_name].factory.get());
}

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

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_UTIL_H_
