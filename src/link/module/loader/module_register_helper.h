/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_REGISTER_HELPER_HPP_
#define LINK_MODULE_LOADER_REGISTER_HELPER_HPP_

#include "link/module/loader/module_register.h"

namespace link {
namespace module {

#define MODULE_REGISTER_INTERNAL(UserModuleClass, ModuleBase, UniqueID)                 \
namespace {                                                                             \
  struct Proxy##UserModuleClass_##UniqueID {                                            \
    Proxy##UserModuleClass_##UniqueID() {                                               \
        link::module::ModuleRegister::CreateModuleFactory<UserModuleClass, ModuleBase>( \
          #UserModuleClass, #ModuleBase);                                               \
    }                                                                                   \
  };                                                                                    \
  static Proxy##UserModuleClass_##UniqueID g_UserModuleRegister_##UniqueID;             \
}

// register class macro
#define MODULE_REGISTER(UserModuleClass, ModuleBase) \
  MODULE_REGISTER_INTERNAL(UserModuleClass, ModuleBase, __COUNTER__)

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_REGISTER_HELPER_HPP_
