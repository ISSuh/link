/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_CONTROLLER_MODULE_CONTROLLER_H_
#define LINK_MODULE_CONTROLLER_MODULE_CONTROLLER_H_

#include <vector>
#include <memory>

#include "link/module/ModuleBase.hpp"
#include "link/module/loader/ModuleLoaderManager.hpp"

namespace link {
namespace module {

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

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_CONTROLLER_MODULE_CONTROLLER_H_
