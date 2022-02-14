/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULEOPENER_HPP_
#define LINK_MODULE_LOADER_MODULEOPENER_HPP_

#include <dlfcn.h>

#include <string>
#include <vector>
#include <memory>

namespace link {
namespace module {

class ModuleOpener {
 public:
  static void moduleOpen(
    const std::string& path, int mode = RTLD_LAZY | RTLD_GLOBAL);

 private:
  static void dlHandleDeletor(uint8_t* hadle);

 private:
  static std::vector<
    std::unique_ptr<uint8_t, std::function<void(uint8_t*)>>> m_dlHandles;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULEOPENER_HPP_
