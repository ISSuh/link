/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/launcher/loader/module_loader.h"

#include <algorithm>
#include <mutex>
#include <functional>

#include "link/launcher/module/user_module.h"
#include "link/launcher/loader/module_loader_util.h"
#include "link/launcher/loader/module_opener.h"

namespace link {
namespace launcher {

void ModuleLoader::LoadModule(const std::string& info) {
  const std::string module_name = info.module_name();
  const std::string module_path = info.module_path();
  const std::string module_class_name = info.class_name();

  // m_moduleInfomationsMap[module_name] = info;

  // link::loader::ModuleOpener::moduleOpen(module_path);

  std::unique_ptr<Module<UserModule>> module =
    Module::CreateModule(module_class_name);
  // module->setArguments(m_moduleInfomationsMap[module_name].getModuleArgs());

  modules_[module_name] = std::move(module);
}

void ModuleLoader::UnLoadModule(const std::string& module_name) {
  std::cout << "ModuleLoader::UnLoadModule - " << module_name << std::endl;

  DistroyModule(module_name);
}

std::shared_ptr<Base> ModuleLoader::GetModule(
  const std::string& module_name) const {
  if (!HasModule(module_name)) {
    return nullptr;
  }
  return modules_.at(module_name);
}

bool ModuleLoader::HasModule(const std::string& module_name) const {
  if (modules_.find(module_name) != modules_.end()) {
    return true;
  }
  return false;
}

std::shared_ptr<UserModule> ModuleLoader::CreateModule(
    const std::string& module_class_name) {
  std::cout << "ModuleLoader::createModuleObject - "
            << module_class_name << std::endl;

  const AbstractModlueFactory<UserModule>* factory =
    ModuleRegister::GetModuleFactory<UserModule>(module_class_name);

  if (!factory) {
    std::cout << "ModuleLoader::createModuleObject - "
              << module_class_name << " load fail" << std::endl;
    return nullptr;
  }

  // std::lock_guard<std::mutex> lock(m_loadedModuleCount_mutex);
  // std::shared_ptr<Base> moduleObject = std::shared_ptr<Base>();
  return std::move(factory->createModule());
}

void ModuleLoader::DistroyModule(const std::string& module_name) {
  std::cout << "ModuleLoader::distroyModuleObject - "
            << module_name << std::endl;

  if (HasModule(module_name)) {
    modules_[module_name].reset();
    modules_.erase(module_name);
  }
}

}  // namespace launcher
}  // namespace link
