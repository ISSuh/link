/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_USER_MODULE_H_
#define LINK_MODULE_BASE_USER_MODULE_H_

#include <string>

#include "link/module/base/user_module_base.h"
#include "link/module/loader/module_register_helper.h"

namespace link {
namespace module {

class UserModule : public UserModuleBase {
 public:
  explicit UserModule(
    const std::string& module_name, ModuleClient* client);
  virtual ~UserModule();

  void Initialize(const base::Json& arguments) override;
  void Process() override;
  void Terminate() override;

  template<typename T>
  void GetArgument(const std::string& key, T* dest);

 protected:
  virtual void Init() = 0;
  virtual void Run() = 0;
  virtual void Shutdown() = 0;

  base::Json arguments_;
};

template<typename T>
void UserModule::GetArgument(const std::string& key, T* dest) {
  *dest = arguments_[key].get<T>();
}

// Implement construct
#define MODULE_CONSTRUCTOR(UserModuleClass)                \
  UserModuleClass(                                        \
    const std::string& module_name,                       \
    link::module::ModuleClient* client)   \
    : link::module::UserModule(module_name, client) {}

// register class macro
#define REGIST_MODULE(UserModuleClass)    \
  MODULE_REGISTER(UserModuleClass, link::module::UserModule)

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_BASE_USER_MODULE_H_
