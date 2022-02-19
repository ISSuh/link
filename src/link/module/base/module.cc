/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/base/module.h"

#include <dlfcn.h>

#include <string>
#include <memory>
#include <utility>

#include "link/module/base/user_module.h"
#include "link/module/loader/module_factory.h"
#include "link/base/logging.h"

namespace link {
namespace module {

void ModuleDeleter(Module* module) {
  delete module;
}

class ModuleHandle {
 public:
  ModuleHandle();
  ~ModuleHandle();

  bool Open(const std::string& path, int32_t flags);

 private:
  uint8_t* handle_;
};

template<typename UserModuleBaseType>
class ModuleImpl : public Module {
 public:
  ModuleImpl(ModuleHandle* module_handle,
         UserModuleBaseType* user_module,
         const Specification& spec)
    : Module(spec),
      module_handle_(module_handle),
      module_(user_module) {}

  virtual ~ModuleImpl() = default;

  void Initialize() override;
  void Process() override;
  void Shutdown() override;

 private:
  std::unique_ptr<ModuleHandle> module_handle_;
  std::unique_ptr<UserModuleBaseType> module_;
};

ModuleHandle::ModuleHandle()
  : handle_(nullptr) {}

ModuleHandle::~ModuleHandle() {
  if (handle_) {
    dlclose(static_cast<void*>(handle_));
  }
}

bool ModuleHandle::Open(const std::string& path, int32_t flags) {
  handle_ = static_cast<uint8_t*>(dlopen(path.c_str(), flags));
  if (!handle_) {
    LOG(ERROR) << __func__ << " - " << dlerror();
    return false;
  }
  return true;
}

Module::Module(const Specification& spec)
  : spec_(spec) {
}

const std::string Module::name() const {
  return spec_.name();
}

const std::string Module::path() const {
  return spec_.path();
}

const std::string Module::class_name() const {
  return spec_.class_name();
}

ModulePtr Module::CreateModule(const Specification& spec) {
  ModuleHandle* module_handle = new ModuleHandle();
  if (!module_handle->Open(spec.path(), RTLD_LAZY | RTLD_GLOBAL)) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name() << " load fail";
    return nullptr;
  }

  const AbstractModlueFactory<UserModuleBase>* factory =
    ModuleRegister::GetInstance()->GetModuleFactory<UserModuleBase>(
      spec.class_name());

  if (!factory) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name() << " can not find factory";
    return nullptr;
  }

  ModulePtr module_impl(
    new ModuleImpl<UserModuleBase>(
      module_handle, factory->CreateModuleObject(), spec),
      &ModuleDeleter);
  return module_impl;
}

template <typename UserModuleBaseType>
void ModuleImpl<UserModuleBaseType>::Initialize() {
  module_->Initialize();
}

template <typename UserModuleBaseType>
void ModuleImpl<UserModuleBaseType>::Process() {
  module_->Process();
}
template <typename UserModuleBaseType>
void ModuleImpl<UserModuleBaseType>::Shutdown() {
  module_->Shutdown();
}

// ModulePtr::ModulePtr(Module* module)
//  : deleter_(std::bind(&ModulePtr::ModuleDeleter, this, std::placeholders::_1)),
//    module_(module, deleter_) {
// }

// ModulePtr::ModulePtr(const ModulePtr& module_ptr) {
//   deleter_ = std::bind(&ModulePtr::ModuleDeleter, this, std::placeholders::_1);
//   module_ptr.deleter_.
//   module_ptr.module_ = std::move(module_ptr.module_);
// }

// ModulePtr::ModulePtr(ModulePtr&& module_ptr) {

// }

// ModulePtr& ModulePtr::operator=(const ModulePtr& module_ptr);
// ModulePtr& ModulePtr::operator=(ModulePtr&& module_ptr);

// ModulePtr::ModuleDeleter() {
//   module_.reset();
// }

}  // namespace module
}  // namespace link
