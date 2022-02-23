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

void ModuleDeleter(LinkModule* module) {
  delete module;
}

class LinkModuleHandle {
 public:
  LinkModuleHandle();
  ~LinkModuleHandle();

  bool Open(const std::string& path, int32_t flags);

 private:
  uint8_t* handle_;
};

LinkModuleHandle::LinkModuleHandle()
  : handle_(nullptr) {}

LinkModuleHandle::~LinkModuleHandle() {
  if (handle_) {
    dlclose(static_cast<void*>(handle_));
  }
}

bool LinkModuleHandle::Open(const std::string& path, int32_t flags) {
  handle_ = static_cast<uint8_t*>(dlopen(path.c_str(), flags));
  if (!handle_) {
    LOG(ERROR) << __func__ << " - " << dlerror();
    return false;
  }
  return true;
}

template<typename UserModuleBaseType>
class LinkModuleImpl : public LinkModule {
 public:
  LinkModuleImpl(LinkModuleHandle* module_handle,
         UserModuleBaseType* user_module,
         const Specification& spec)
    : LinkModule(spec),
      module_handle_(module_handle),
      module_(user_module) {}

  virtual ~LinkModuleImpl() = default;

  void Initialize() override;
  void Process() override;
  void Shutdown() override;

 private:
  std::unique_ptr<LinkModuleHandle> module_handle_;
  std::unique_ptr<UserModuleBaseType> module_;
};

template <typename UserModuleBaseType>
void LinkModuleImpl<UserModuleBaseType>::Initialize() {
  module_->Initialize();
}

template <typename UserModuleBaseType>
void LinkModuleImpl<UserModuleBaseType>::Process() {
  module_->Process();
}
template <typename UserModuleBaseType>
void LinkModuleImpl<UserModuleBaseType>::Shutdown() {
  module_->Shutdown();
}

LinkModule::LinkModule(const Specification& spec)
  : spec_(spec) {
}

const std::string LinkModule::name() const {
  return spec_.name();
}

const std::string LinkModule::path() const {
  return spec_.path();
}

const std::string LinkModule::class_name() const {
  return spec_.class_name();
}

LinkModulePtr LinkModule::CreateModule(const Specification& spec) {
  LinkModuleHandle* module_handle = new LinkModuleHandle();
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

  LinkModulePtr module_impl(
    new LinkModuleImpl<UserModuleBase>(
      module_handle, factory->CreateModuleObject(), spec),
      &ModuleDeleter);
  return module_impl;
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
