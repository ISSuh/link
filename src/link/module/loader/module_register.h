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

namespace nlink {
namespace module {

class ModuleRegister {
 public:

  // TODO(issuh) : should modify referece coutner for AbstractModuleFactoryBase
  struct FactoryWrapper {
    uint32_t ref_count = 0;
    AbstractModuleFactoryBase* factory = nullptr;
  };

  static ModuleRegister* GetInstance();

  template<typename UserModuleImpl, typename UserModule>
  void CreateModuleFactory(
    const std::string& class_name, const std::string& base_class_name);

  void ReleaseModuleFactory(const std::string& class_name);

  template <typename UserModule>
  AbstractModlueFactory<UserModule>* GetModuleFactory(
    const std::string& class_name);

 private:
  static std::atomic<ModuleRegister*> instance_;
  static std::mutex lock_;

  using ModuleFactoryMap = std::map<std::string, FactoryWrapper>;
  ModuleFactoryMap factories_;
};

template<typename UserModuleImpl, typename UserModule>
void ModuleRegister::CreateModuleFactory(const std::string& class_name,
                                        const std::string& base_class_name) {
  if (factories_.find(class_name) != factories_.end()) {
    ++factories_[class_name].ref_count;
    return;
  }

  AbstractModuleFactoryBase* factory(
    new ModuleFactory<UserModuleImpl, UserModule>(
        class_name, base_class_name));

  FactoryWrapper factory_wrapper;
  factory_wrapper.ref_count = 1;
  factory_wrapper.factory = factory;
  factories_.insert({class_name, factory_wrapper});
}

template <typename UserModule>
AbstractModlueFactory<UserModule>*
  ModuleRegister::GetModuleFactory(const std::string& class_name) {
  if (factories_.find(class_name) == factories_.end()) {
    return nullptr;
  }

  return static_cast<AbstractModlueFactory<UserModule>*>(
    factories_[class_name].factory);
}

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_UTIL_H_
