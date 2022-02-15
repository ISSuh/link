/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_USER_MODULE_BASE_H_
#define LINK_MODULE_BASE_USER_MODULE_BASE_H_

#include <iostream>
#include <memory>
#include <atomic>

namespace link {
namespace module {

class UserModuleBase
  : public std::enable_shared_from_this<UserModuleBase> {
 public:
  UserModuleBase() = default;
  virtual ~UserModuleBase() = default;

  virtual bool Initialize() { return false; }
  virtual bool Process() { return false; }
  virtual bool Shutdown() { return false; }

 private:
  // std::atomic<bool> m_isShutdown = { false };
  // std::string m_moduleName;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_BASE_USER_MODULE_BASE_H_
