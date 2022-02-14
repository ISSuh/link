/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_loader.h"

#include <algorithm>
#include <mutex>
#include <functional>

#include "link/module/module.h"
#include "link/module/loader/module_register.h"
#include "link/base/logging.h"

namespace link {
namespace module {

void ModuleLoader::LoadModule(const std::string& spec_json) {
  Module::Specification spec;
  spec.ParseFromStr(spec_json);

  const std::string module_name = spec.name;
  if (modules_.find(module_name) != modules_.end()) {
    LOG(WARN) << __func__ << " - already loaded " << module_name;
    return;
  }

  std::unique_ptr<Module> module = Module::CreateModule(spec);
  modules_[module_name] = std::move(module);
}

void ModuleLoader::UnLoadModule(const std::string& module_name) {
  std::cout << "ModuleLoader::UnLoadModule - " << module_name << std::endl;
  if (HasModule(module_name)) {
    const std::string class_name = modules_[module_name]->class_name();
    modules_[module_name].reset();
    modules_.erase(module_name);

    ModuleRegister::ReleaseModuleFactory(class_name);
  }
}

Module* ModuleLoader::GetModule(const std::string& module_name) const {
  if (!HasModule(module_name)) {
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

}  // namespace module
}  // namespace link
