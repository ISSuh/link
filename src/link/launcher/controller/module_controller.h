/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_CONTROLLER_MODULE_CONTROLLER_H_
#define LINK_LAUNCHER_CONTROLLER_MODULE_CONTROLLER_H_

#include <vector>
#include <memory>

#include "link/launcher/module/ModuleBase.hpp"
#include "link/launcher/loader/ModuleLoaderManager.hpp"

namespace link {
namespace launcher {

class ModuleController {
 public:
  ModuleController();

  bool Init();
  void Clear();

 private:
  bool LoadAllModule();
  // bool LoadMoule(const util::json& arg);

 private:
  smt::loader::ModuleLoadManager m_loadManager;
};

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_CONTROLLER_MODULE_CONTROLLER_H_
