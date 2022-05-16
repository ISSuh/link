/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULE_LOADER_H_
#define LINK_MODULE_LOADER_MODULE_LOADER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>

#include "link/module/base/module.h"
#include "link/module/base/specification.h"
#include "link/base/macro.h"

namespace nlink {
namespace module {

class ModuleLoader {
 public:
  ModuleLoader() = default;
  ~ModuleLoader() = default;

  bool LoadModule(ModuleClient* client, const Specification& spec);
  void UnLoadModule(const std::string& module_name);

  LinkModule* GetModule(const std::string& module_name) const;
  bool HasModule(const std::string& module_name) const;

  const std::vector<std::string> ModuleNames() const;

 private:
  std::map<std::string, LinkModulePtr> modules_;

  DISAALOW_COPY_AND_ASSIGN(ModuleLoader)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_H_
