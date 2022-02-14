/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULE_LOADER_MANAGER_H_
#define LINK_MODULE_LOADER_MODULE_LOADER_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "link/module.h"
#include "link/module/loader/module_loader.h"
#include "link/base/json_wrapper.h"

namespace link {
namespace module {

using LoaderId = uint64_t;

class ModuleLoadManager {
 public:
  ModuleLoadManager();
  ~ModuleLoadManager();

  void CreateModlueLoader(base::JsonWrapper specifications);

  void loadModule(LoaderId id, const module::ModuleInfo& moduleName);
  void loadAllModule();
  void unLoadModule(LoaderId id, const ModuleName& moduleName);
  void unLoadAllModule();

  std::shared_ptr<UserModule> getModule(const ModuleName& moduleName);
  bool hasModuleLoader(LoaderId id) const;
  bool hasModule(const ModuleName& modulName) const;

  LoaderId getLoaderIdByModuleName(const ModuleName& name) const {
    return m_loaderIdByModuleName.at(name);
  }

 private:
  void paraseModuleSpecifications(LoaderId id, util::JsonWrapper specification);

 private:
  std::map<LoaderId, std::unique_ptr<ModuleLoader> module_loades_;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_MANAGER_H_
