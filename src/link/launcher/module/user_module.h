/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_USER_MODULE_MODULE_H_
#define LINK_LAUNCHER_USER_MODULE_MODULE_H_

#include <string>
#include <memory>
#include <algorithm>

#include "link/launcher/module/module_base.h"
#include "link/launcher/module/module_register.h"
#include "link/base/json_wrapper.h"

namespace link {
namespace launcher {

class UserModule : public ModuleBase {
 protected:
  virtual void Init() = 0;
  virtual void Run() = 0;
  virtual void Terminate() = 0;

 public:
  UserModule() = default;
  virtual ~UserModule() = default;

  bool Initialize() override {
    Init();
    return true;
  }

  bool Process() override {
    Run();
    return true;
  }

  bool Shutdown() override {
    Terminate();
    return true;
  }

  // void setArguments(util::JsonWrapper args) {
  //   m_arguments = args;
  // }

  // template<typename T>
  // void GetArgument(const std::string& args, T& var) {
  //   const T temp = m_arguments[args].get<T>();
  //   std::copy(&temp, &temp + sizeof(temp), &var);
  // }

 private:
  // util::JsonWrapper m_arguments;
};

// register class macro
#define REGIST_MODULE(UserModuleClass) \
  MODULE_REGISTER(UserModuleClass, link::launcher::UserModule)

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_USER_MODULE_MODULE_H_
