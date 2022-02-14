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

#include "link/module/module.h"
#include "link/base/macro.h"

namespace link {
namespace module {

class ModuleLoader {
 public:
  void LoadModule(const std::string& spec_json);
  void UnLoadModule(const std::string& module_name);

  Module* GetModule(const std::string& module_name) const;
  bool HasModule(const std::string& module_name) const;

 private:
  std::map<std::string, std::unique_ptr<Module>> modules_;

  DISAALOW_COPY_AND_ASSIGN(ModuleLoader)
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULE_LOADER_H_
