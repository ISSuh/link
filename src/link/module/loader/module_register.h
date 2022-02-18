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
#include <atomic>
#include <mutex>

#include "link/module/loader/module_factory.h"
#include "link/base/logging.h"

namespace link {
namespace module {

class ModuleRegister {
 public:
  struct FactoryWrapper {
    uint32_t ref_count = 0;
    std::unique_ptr<AbstractModuleFactoryBase> factory = nullptr;
  };

  static ModuleRegister* GetInstance();

  template<typename UserModuleType, typename UserModuleBaseType>
  void CreateModuleFactory(
    const std::string& class_name, const std::string& base_class_name);

  void ReleaseModuleFactory(const std::string& class_name);

  template <typename UserModuleBaseType>
  AbstractModlueFactory<UserModuleBaseType>* GetModuleFactory(
    const std::string& class_name);

 private:
  static std::atomic<ModuleRegister*> instance_;
  static std::mutex lock_;

  using ModuleFactoryMap = std::map<std::string, FactoryWrapper>;
  ModuleFactoryMap factories_;
};

template<typename UserModuleType, typename UserModuleBaseType>
void ModuleRegister::CreateModuleFactory(const std::string& class_name,
                                        const std::string& base_class_name) {
  if (factories_.find(class_name) != factories_.end()) {
    ++factories_[class_name].ref_count;
    return;
  }

  std::unique_ptr<AbstractModuleFactoryBase> factory(
    new ModuleFactory<UserModuleType, UserModuleBaseType>(
        class_name, base_class_name));

  FactoryWrapper factory_wrapper;
  factory_wrapper.ref_count = 1;
  factory_wrapper.factory = std::move(factory);
  factories_.insert({class_name, std::move(factory_wrapper)});
}

template <typename UserModuleBaseType>
AbstractModlueFactory<UserModuleBaseType>*
  ModuleRegister::GetModuleFactory(const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return nullptr;
  }

  // TODO(issuh) : why do not use dynamic_cast????
  LOG(ERROR) << __func__ << " - addr : "<< factories_[class_name].factory.get()
              << " / "
              << dynamic_cast<AbstractModlueFactory<UserModuleBaseType>*>(factories_[class_name].factory.get())
              << " / "
              << static_cast<AbstractModlueFactory<UserModuleBaseType>*>(factories_[class_name].factory.get())
              << " / "
              << reinterpret_cast<AbstractModlueFactory<UserModuleBaseType>*>(factories_[class_name].factory.get());

  return static_cast<AbstractModlueFactory<UserModuleBaseType>*>(
    factories_[class_name].factory.get());
}

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_UTIL_H_
