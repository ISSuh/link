/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_USER_MODULE_BASE_H_
#define LINK_MODULE_BASE_USER_MODULE_BASE_H_

#include <string>

#include "link/base/json_wrapper.h"

namespace link {
namespace module {

class UserModuleBase {
 public:
  class ModuleClient {
    virtual void OnTerminate() = 0;
  };

  UserModuleBase(
    const std::string& module_name, UserModuleBase::ModuleClient* client);
  virtual ~UserModuleBase();

  virtual void Initialize(const base::Json& arguments) = 0;
  virtual void Process() = 0;
  virtual void Terminate() = 0;

  bool IsRunning() const;

 protected:
  const std::string module_name_;
  bool running_state_;
  ModuleClient* client_;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_BASE_USER_MODULE_BASE_H_
