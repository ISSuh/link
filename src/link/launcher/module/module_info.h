/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_LAUNCHER_MODULE_MODULE_INFO_H_
#define LINK_LAUNCHER_MODULE_MODULE_INFO_H_

#include <string>

#include "link/base/json_wrapper.h"

namespace link {
namespace launcher {

class BaseModuleInfo {
 public:
  BaseModuleInfo(
    const std::string& base_module_name, const std::string& base_class_name)
      : base_module_name_(baseModuleName), base_class_name_(base_class_name) {}
  virtual ~BaseModuleInfo() = default;

  const std::string base_module_name() const {
    return base_module_name_;
  }

  const std::string base_class_name() const {
    return base_class_name_;
  }

 protected:
  const std::string base_module_name_;
  const std::string base_class_name_;
};

class ModuleInfo : public BaseModuleInfo {
 public:
  ModuleInfo();
  virtual ~ModuleInfo() = default;

  bool parseModueInfoOnJson(base::JsonWrapper info);

  const std::string& module_name() const { return module_name_; }
  const std::string& class_name() const { return class_name_; }
  const std::string& module_path() const {return module_path; }
  const base::JsonWrapper module_args() const { return module_args; }
  const base::JsonWrapper module_configure() const { return module_configure; }

  ModuleInfo operator=(const ModuleInfo& rhs) {
    if (this == &rhs) {
      return *this;
    }

    module_name_ = rhs.module_name_;
    class_name_ = rhs.class_name_;
    module_path_ = rhs.module_path_;
    module_args_ = rhs.module_args_;
    module_configure_ = rhs.module_configure_;

    return *this;
  }

 private:
  void setModuleName(const std::string& moduleName);
  void setClassName(const std::string& className);
  void setModulePath(const std::string& modulePath);
  void setModuleArgs(const util::JsonWrapper& args);
  void setModuleConfigure(const util::JsonWrapper& configures);

 private:
  std::string module_name_;
  std::string class_name_;
  std::string module_path_;
  base::JsonWrapper module_args_;
  base::JsonWrapper module_configure_;
};

}  // namespace launcher
}  // namespace link

#endif  // LINK_LAUNCHER_MODULE_MODULE_INFO_H_
