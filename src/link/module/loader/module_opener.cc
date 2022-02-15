/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/loader/module_opener.h"

#include <iostream>

#include "link/module/base/user_modul_base.h"

namespace link {
namespace module {

std::vector<std::unique_ptr<
  uint8_t, std::function<void(uint8_t*)>>> ModuleOpener::m_dlHandles;

void ModuleOpener::moduleOpen(const std::string& path, int mode) {
  std::cout << "ModuleOpener::moduleOpen - " << path << std::endl;

  uint8_t* moduleHandle = static_cast<uint8_t*>(dlopen(path.c_str(), mode));
  if (moduleHandle == nullptr) {
    std::cerr << dlerror() << std::endl;
    exit(1);
  }

  m_dlHandles.emplace_back(
    std::move(moduleHandle), ModuleOpener::dlHandleDeletor);
}

void ModuleOpener::dlHandleDeletor(uint8_t* hadle) {
  dlclose(static_cast<void*>(hadle));
}

}  // namespace module
}  // namespace link
