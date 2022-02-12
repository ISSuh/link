/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/launcher/loader/module_loader_util.h"

namespace link {
namespace launcher {

ModuleRegister::ModuleFactoryMap ModuleRegister::factories_ =
  ModuleFactoryMap();

}  // namespace launcher
}  // namespace link
