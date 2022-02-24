/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_MODULE_CLIENT_H_
#define LINK_MODULE_BASE_MODULE_CLIENT_H_

#include <string>

namespace link {
namespace module {

class ModuleClient {
 public:
  virtual void AfterInitialize(const std::string& module_name) = 0;
  virtual void AfterProcess(const std::string& module_name) = 0;
  virtual void AfterTerminate(const std::string& module_name) = 0;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_BASE_MODULE_CLIENT_H_
