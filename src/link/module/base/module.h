/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_MODULE_H_
#define LINK_MODULE_BASE_MODULE_H_

#include <string>
#include <memory>

#include "link/module/base/specification.h"
#include "link/base/macro.h"
#include "link/base/json_wrapper.h"

namespace link {
namespace module {

class Module;

using ModulePtr =
  std::unique_ptr<Module, std::function<void(Module*)>>;

class Module {
 public:
  virtual ~Module() = default;

  static ModulePtr CreateModule(const Specification& spec);

  const std::string name() const;
  const std::string class_name() const;
  const std::string path() const;

  virtual void Initialize() = 0;
  virtual void Process() = 0;
  virtual void Shutdown() = 0;

 protected:
  explicit Module(const Specification& spec);

  const Specification spec_;

  DISAALOW_COPY_AND_ASSIGN(Module)
};

// class ModulePtr {
//  public:
//   ModulePtr(Module* module);
//   virtual ~ModulePtr() = default;

//   ModulePtr(const ModulePtr& module_ptr);
//   ModulePtr(ModulePtr&& module_ptr);

//   ModulePtr& operator=(const ModulePtr& module_ptr);
//   ModulePtr& operator=(ModulePtr&& module_ptr);

//  private:
//   void ModuleDeleter(Module* module);

//   std::function<void(Module*)> deleter_;
//   std::unique_ptr<Module, std::function<void(Module*)>> module_;
// };

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_BASE_MODULE_H_
