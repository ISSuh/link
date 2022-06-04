/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_USER_MODULE_BASE_H_
#define LINK_MODULE_BASE_USER_MODULE_BASE_H_

#include <string>

#include "link/base/logging.h"
#include "link/base/json_wrapper.h"
#include "link/base/task/task_runner.h"
#include "link/module/base/module_client.h"

namespace nlink {
namespace module {

class UserModuleBase {
 public:
  UserModuleBase(
    const std::string& module_name, ModuleClient* client);
  virtual ~UserModuleBase();

  virtual void Initialize(
    base::TaskRunner* task_runner, const base::Json& arguments) = 0;
  virtual void Process() = 0;
  virtual void Terminate() = 0;

  bool IsRunning() const;

 protected:
  const std::string module_name_;
  bool running_state_;
  ModuleClient* client_;
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_BASE_USER_MODULE_BASE_H_
