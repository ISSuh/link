/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_MODULE_MODULE_REGISTER_HPP_
#define LINK_LAUNCHER_MODULE_MODULE_REGISTER_HPP_

#include "link/launcher/loader/module_loader_util.h"

namespace link {
namespace launcher {

#define MODULE_REGISTER_INTERNAL(UserModuleClass, ModuleBase, UniqueID)                 \
namespace {                                                                             \
  struct Proxy##UserModuleClass_##UniqueID {                                            \
    Proxy##UserModuleClass_##UniqueID() {                                               \
        smt::loader::ModuleLoaderUtil::registUserModule<UserModuleClass, ModuleBase>(   \
          #UserModuleClass, #ModuleBase);                                               \
    }                                                                                   \
  };                                                                                    \
  static Proxy##UserModuleClass_##UniqueID g_registerModule_##UniqueID;                 \
}

// register class macro
#define MODULE_REGISTER(UserModuleClass, ModuleBase) \
  MODULE_REGISTER_INTERNAL(UserModuleClass, ModuleBase, __COUNTER__)

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_MODULE_MODULE_REGISTER_HPP_
