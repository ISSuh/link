/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_LOADER_MODULE_LOADER_H_
#define LINK_LAUNCHER_LOADER_MODULE_LOADER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>

#include "link/launcher/module/module.h"
#include "link/launcher/module/module_info.h"

namespace link {
namespace launcher {

class ModuleLoader {
 public:
  ModuleLoader() = default;
  ~ModuleLoader() = default;

  void LoadModule(const ModuleInfo& info);
  void UnLoadModule(const std::string& module_name);

  std::shared_ptr<Module> GetModule(const std::string& module_name) const;
  bool HasModule(const std::string& module_name) const;

 private:
  std::shared_ptr<Module> CreateModule(const std::string& module_name);
  void DistroyModule(const std::string& module_name);

 private:
  std::map<std::string, std::shared_ptr<Module>> modules_;
};

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_LOADER_MODULE_LOADER_H_
