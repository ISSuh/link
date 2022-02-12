/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_MODULE_MODULEBASE_HPP_
#define LINK_LAUNCHER_MODULE_MODULEBASE_HPP_

#include <iostream>
#include <memory>
#include <atomic>

namespace link {
namespace launcher {

class ModuleBase : public std::enable_shared_from_this<ModuleBase> {
 public:
  ModuleBase() = default;
  virtual ~ModuleBase() = default;

  virtual bool Initialize() { return false; }
  virtual bool Process() { return false; }
  virtual bool Shutdown() { return false; }

 private:
  // std::atomic<bool> m_isShutdown = { false };
  // std::string m_moduleName;
};

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_MODULE_MODULEBASE_HPP_
