/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULE_FACTORY_H_
#define LINK_MODULE_LOADER_MODULE_FACTORY_H_

#include <string>
#include <memory>

#include "link/module/base/user_module_base.h"

namespace link {
namespace module {

class AbstractModuleFactoryBase {
 public:
  AbstractModuleFactoryBase(
    const std::string& class_name, const std::string& base_class_name)
      : class_name_(class_name), base_class_name_(base_class_name) {}

  virtual ~AbstractModuleFactoryBase() = default;

  const std::string base_class_name() const {
    return base_class_name_;
  }

  const std::string class_name() const {
    return class_name_;
  }

 protected:
  const std::string class_name_;
  const std::string base_class_name_;
};

template <typename UserModule>
class AbstractModlueFactory : public AbstractModuleFactoryBase {
 public:
  AbstractModlueFactory(
    const std::string& class_name, const std::string& base_class_name)
      : AbstractModuleFactoryBase(class_name, base_class_name) {}

  virtual UserModule* CreateModuleObject(
    const std::string& module_name,
    UserModuleBase::ModuleClient* client) const = 0;

 private:
  AbstractModlueFactory();
  AbstractModlueFactory(const AbstractModlueFactory &);
  AbstractModlueFactory &operator=(const AbstractModlueFactory &);
};

template <typename UserModuleImpl, typename UserModule>
class ModuleFactory : public AbstractModlueFactory<UserModule> {
 public:
  ModuleFactory(
    const std::string& class_name, const std::string& base_class_name)
      : AbstractModlueFactory<UserModule>(class_name, base_class_name) {}

  UserModule* CreateModuleObject(
    const std::string& module_name,
    UserModuleBase::ModuleClient* client) const {
      return dynamic_cast<UserModule*>(new UserModuleImpl(module_name, client));
  }
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULE_FACTORY_H_
