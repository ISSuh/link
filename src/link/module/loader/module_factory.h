/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_LOADER_MODULE_FACTORY_H_
#define LINK_MODULE_LOADER_MODULE_FACTORY_H_

#include <string>
#include <memory>

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

template <typename UserModuleBase>
class AbstractModlueFactory : public AbstractModuleFactoryBase {
 public:
  AbstractModlueFactory(
    const std::string& class_name, const std::string& base_class_name)
      : AbstractModuleFactoryBase(class_name, base_class_name) {}

  virtual UserModuleBase* CreateModuleObject() const = 0;

 private:
  AbstractModlueFactory();
  AbstractModlueFactory(const AbstractModlueFactory &);
  AbstractModlueFactory &operator=(const AbstractModlueFactory &);
};

template <typename UserModule, typename UserModuleBase>
class ModuleFactory : public AbstractModlueFactory<UserModuleBase> {
 public:
  ModuleFactory(
    const std::string& class_name, const std::string& base_class_name)
      : AbstractModlueFactory<UserModuleBase>(class_name, base_class_name) {}

  UserModuleBase* CreateModuleObject() const {
    return new UserModule();
  }
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_LOADER_MODULE_FACTORY_H_
