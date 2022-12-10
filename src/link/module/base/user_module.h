/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_USER_MODULE_H_
#define LINK_MODULE_BASE_USER_MODULE_H_

#include <string>
#include <memory>

#include "link/base/logger.h"
#include "link/base/task/task_runner.h"
#include "link/module/base/user_module_base.h"
#include "link/module/loader/module_register_helper.h"

namespace nlink {
namespace module {

class UserModule : public UserModuleBase {
 public:
  explicit UserModule(
    const std::string& module_name, ModuleClient* client);
  virtual ~UserModule();

  void Initialize(
    std::weak_ptr<base::TaskRunner> task_runner,
    const base::Json& arguments) override;

  void Process() override;

  void Terminate() override;

  template<typename T>
  void GetArgument(const std::string& key, T* dest);

  std::weak_ptr<base::TaskRunner> GetTaskRunner() const;

 protected:
  virtual void Init() = 0;
  virtual void Run() = 0;
  virtual void Shutdown() = 0;

  std::weak_ptr<base::TaskRunner> task_runner_weak_;
  std::shared_ptr<base::Logger> logger_;

  base::Json arguments_;
};

template<typename T>
void UserModule::GetArgument(const std::string& key, T* dest) {
  *dest = arguments_[key].get<T>();
}

// Implement construct
#define MODULE_CONSTRUCTOR(UserModuleClass)             \
  UserModuleClass(                                      \
    const std::string& module_name,                     \
    nlink::module::ModuleClient* client)                 \
    : nlink::module::UserModule(module_name, client) {}

// register class macro
#define REGIST_MODULE(UserModuleClass)    \
  MODULE_REGISTER(UserModuleClass, nlink::module::UserModule)

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_BASE_USER_MODULE_H_
