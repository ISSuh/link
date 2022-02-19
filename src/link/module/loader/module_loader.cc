/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_loader.h"

#include <algorithm>
#include <mutex>
#include <functional>

#include "link/module/loader/module_register.h"
#include "link/base/logging.h"

namespace link {
namespace module {

bool ModuleLoader::LoadModule(const Specification& spec) {
  LOG(INFO) << __func__ << " - " << spec.name();

  const std::string module_name = spec.name();
  if (modules_.find(module_name) != modules_.end()) {
    LOG(WARN) << __func__ << " - already loaded " << module_name;
    return false;
  }

  ModulePtr module = Module::CreateModule(spec);
  if (!module) {
    return false;
  }

  modules_[module_name] = std::move(module);
  return true;
}

void ModuleLoader::UnLoadModule(const std::string& module_name) {
  LOG(INFO) << __func__ << " - " << module_name;

  if (HasModule(module_name)) {
    const std::string class_name = modules_[module_name]->class_name();
    modules_[module_name].reset();
    modules_.erase(module_name);

    ModuleRegister::GetInstance()->ReleaseModuleFactory(class_name);
  }
}

Module* ModuleLoader::GetModule(const std::string& module_name) const {
  if (!HasModule(module_name)) {
    LOG(INFO) << __func__ << " - can not find module " << module_name;
    return nullptr;
  }
  return modules_.at(module_name).get();
}

bool ModuleLoader::HasModule(const std::string& module_name) const {
  if (modules_.find(module_name) != modules_.end()) {
    return true;
  }
  return false;
}

const std::vector<std::string> ModuleLoader::ModuleNames() const {
  std::vector<std::string> names;
  for (const auto& module : modules_) {
    names.emplace_back(module.first);
  }
  return names;
}

}  // namespace module
}  // namespace link
