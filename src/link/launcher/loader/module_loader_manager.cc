/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/launcher/loader/module_loader_manager.h"

#include <iostream>
#include <utility>

// #include "link/launcher/util/IdGenerator.h"

namespace link {
namespace launcher {

ModuleLoadManager::ModuleLoadManager() {}

ModuleLoadManager::~ModuleLoadManager() {}

void ModuleLoadManager::CreateModlueLoader(base::JsonWrapper specifications) {
  std::cout << "ModuleLoadManager::createModlueLoader\n";

  LoaderId id = util::idGenerator();
  paraseModuleSpecifications(id, specifications);

  module_loades_[id] = std::unique_ptr<ModuleLoader>(
      new ModuleLoader());
}

void ModuleLoadManager::loadModule(
  LoaderId id, const module::ModuleInfo& info) {
  module_loades_[id]->loadModule(info);
}

void ModuleLoadManager::loadAllModule() {
  for (const auto& moduleInfoByid : m_moduleInfomationsByLoader) {
    LoaderId id = moduleInfoByid.first;
    for (const module::ModuleInfo moduleInfo : moduleInfoByid.second) {
      loadModule(id, moduleInfo);
    }
  }
}

void ModuleLoadManager::unLoadModule(
  LoaderId id, const ModuleName& moduleName) {
  module_loades_[id]->unLoadModule(moduleName);
}

void ModuleLoadManager::unLoadAllModule() {
  for (const auto& moduleInfoByid : m_moduleInfomationsByLoader) {
    LoaderId id = moduleInfoByid.first;
    for (const module::ModuleInfo moduleInfo : moduleInfoByid.second) {
      ModuleName name = moduleInfo.getModuleName();
      unLoadModule(id, name);
    }
  }
}

std::shared_ptr<module::Module> ModuleLoadManager::getModule(
  const ModuleName& moduleName) {
  LoaderId id = m_loaderIdByModuleName[moduleName];
  std::cout << "ModuleLoadManager::getModule - "
            << id << " / " << moduleName << std::endl;
  return std::move(module_loades_[id]->getModule(moduleName));
}

bool ModuleLoadManager::hasModuleLoader(LoaderId id) const {
  if (module_loades_.find(id) != module_loades_.end()) {
    return true;
  }
  return false;
}

bool ModuleLoadManager::hasModule(const ModuleName& modulName) const {
  if (m_loaderIdByModuleName.find(modulName) == m_loaderIdByModuleName.end()) {
    return false;
  }

  LoaderId id = m_loaderIdByModuleName.at(modulName);
  if (module_loades_.at(id)->hasModule(modulName)) {
    return true;
  }

  return false;
}


void ModuleLoadManager::paraseModuleSpecifications(
  LoaderId id, util::JsonWrapper specifications) {
  ModuleInfomations infomations;
  util::JsonWrapper modules(specifications[util::JSONKEY::MODULES]);
  for (uint32_t i = 0 ; i < modules.size() ; ++i) {
    module::ModuleInfo info;
    info.parseModueInfoOnJson(modules[i]);

    ModuleName name = info.getModuleName();
    m_loaderIdByModuleName[name] = id;

    infomations.emplace_back(info);
  }

  m_moduleInfomationsByLoader[id] = std::move(infomations);
}

}  // namespace launcher
}  // namespace link
