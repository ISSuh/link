/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_register.h"

namespace link {
namespace module {

ModuleRegister::ModuleFactoryMap ModuleRegister::factories_
  = ModuleFactoryMap();

}  // namespace module
}  // namespace link
