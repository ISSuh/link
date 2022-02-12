/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_MODULE_MODULE_H_
#define LINK_LAUNCHER_MODULE_MODULE_H_

#include <dlfcn.h>

#include <string>
#include <memory>
#include <utility>

#include "link/launcher/module/module_info.h"
#include "link/launcher/loader/module_factory.h"
#include "base/logging.h"

namespace link {
namespace launcher {

class ModuleHandle {
 public:
  // using Handle = std::unique_ptr<uint8_t, std::function<void(uint8_t*)>
  ModuleHandle()
    : handle_(nullptr) {}

  bool Open(const std::string& path, int32_t flags) {
    handle_ = static_cast<uint8_t*>(dlopen(path.c_str(), flags));
    if (!handle_) {
      std::cerr << dlerror() << std::endl;
      return false;
    }
    return true;
  }

  ~ModuleHandle() {
    if (handle_) {
      dlclose(static_cast<void*>(handle_));
    }
  }

 private:
  uint8_t* handle_;
}

template<typename UserModuleBase>
class Module {
 public:
  static std::unique_ptr<Module<UserModuleBase>> CreateModule(
    ModuleInfo module_spec, int32_t flag = RTLD_LAZY | RTLD_GLOBAL);

 private:
  Module(ModuleHandle* module_handle,
         UserModuleBase* user_module,
         ModuleInfo spec)
    : module_handle_(module_handle), module_(user_module), spec_(spec) {}

  ~Module();

  std::unique_ptr<ModuleHandle> module_handle_;
  std::unique_ptr<UserModuleBase> module_;
  ModuleInfo spec_;
};

std::unique_ptr<Module<UserModuleBase>> Module::CreateModule(
  ModuleInfo module_spec, int32_t flag = RTLD_LAZY | RTLD_GLOBAL) {
  const std::string path = module_spec.module_path();
  const std::string class_name = module_spec.class_name();

  ModuleHandle module_handle = new ModuleHandle();
  if (!module_handle->Open(path, flag)) {
    LOG(ERROR) << __func__ << " - "
               << class_name << " load fail";
    return nullptr;
  }

  const AbstractModlueFactory<UserModuleBase>* factory =
    ModuleRegister::GetModuleFactory<UserModuleBase>(class_name);
  if (!factory) {
    LOG(ERROR) << __func__ << " - "
               << class_name << " can not find factory";
    return nullptr;
  }

  std::unique_ptr<Module<UserModuleBase>> user_module(
    module_handle, factory->CreateModuleObject(), module_spec)
  return std::move(user_module);
}

template<typename UserModuleBase>
class ModuleImpl {
 public:
  
 private:
  std::unique_ptr<ModuleHandle> module_handle_;
  std::unique_ptr<UserModuleBase> module_;
  ModuleInfo spec_;
}

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_MODULE_MODULE_H_
