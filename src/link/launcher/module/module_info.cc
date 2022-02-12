/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/launcher/module/module_info.h"

#include <iostream>
#include <string>

namespace link {
namespace launcher {

ModuleInfo::ModuleInfo()
  : BaseModuleInfo("Module", "Module"),
    module_name_(""),
    class_name_(""),
    module_path_(""),
    module_args_(),
    module_configure_() {}

bool ModuleInfo::parseModueInfoOnJson(util::JsonWrapper info) {
  if (!info.hasKey(util::JSONKEY::MODULE_NAME)) {
    std::cout << "ModuleInfo::parseModueInfoOnJson - "
              << " Parse Error. Chek json key [module_name]" << std::endl;
    return false;
  }
  setModuleName(info[util::JSONKEY::MODULE_NAME].get<std::string>());

  if (!info.hasKey(util::JSONKEY::MODULE_PATH)) {
    std::cout << "ModuleInfo::parseModueInfoOnJson - "
              << " Parse Error. Chek json key [module_path]" << std::endl;
    return false;
  }
  setModulePath(info[util::JSONKEY::MODULE_PATH].get<std::string>());

  if (!info.hasKey(util::JSONKEY::MODULE)) {
    std::cout << "ModuleInfo::parseModueInfoOnJson - "
              << " Parse Error. Chek json key [Module]" << std::endl;
    return false;
  }

  util::JsonWrapper moduleInfo(info[util::JSONKEY::MODULE]);

  if (!moduleInfo.hasKey(util::JSONKEY::CLASS_NAME)) {
    std::cout << "ModuleInfo::parseModueInfoOnJson - "
              << " Parse Error. Chek json key [class_name]" << std::endl;
    return false;
  }
  setClassName(info[util::JSONKEY::MODULE][util::JSONKEY::CLASS_NAME].get<std::string>());

  if (moduleInfo.hasKey(util::JSONKEY::ARGS)) {
    setModuleArgs(moduleInfo[util::JSONKEY::ARGS]);
  }

  if (moduleInfo.hasKey(util::JSONKEY::CONFIGURE)) {
    setModuleConfigure(moduleInfo[util::JSONKEY::CONFIGURE]);
  }

  return true;
}

void ModuleInfo::setModuleName(const std::string& moduleName) {
  module_name_ = moduleName;
}
void ModuleInfo::setClassName(const std::string& className) {
  class_name_ = className;
}
void ModuleInfo::setModulePath(const std::string& modulePath) {
  module_path_ = modulePath;
}
void ModuleInfo::setModuleArgs(const util::JsonWrapper& args) {
  module_args_ = args;
}
void ModuleInfo::setModuleConfigure(const util::JsonWrapper& configures) {
  module_configure_ = configures;
}

}  // namespace launcher
}  // namespace link
