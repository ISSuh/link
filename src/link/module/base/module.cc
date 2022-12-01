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

namespace nlink {
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

LinkModule::LinkModule(const Specification& spec)
  : spec_(spec) {
}

const Specification LinkModule::ModuleSpecification() const {
  return spec_;
}

template<typename UserModule>
class LinkModuleImpl : public LinkModule {
 public:
  LinkModuleImpl(
    LinkModuleHandle* module_handle,
    UserModule* user_module,
    std::weak_ptr<base::TaskRunner> task_runner_weak,
    const Specification& spec)
    : LinkModule(spec),
      module_handle_(module_handle),
      module_(user_module),
      task_runner_weak_(task_runner_weak) {}

  virtual ~LinkModuleImpl() = default;

  void Initialize() override;
  void Process() override;
  void Terminate() override;
  bool IsRunning() const override;

 private:
  std::unique_ptr<LinkModuleHandle> module_handle_;
  std::unique_ptr<UserModule> module_;
  std::weak_ptr<base::TaskRunner> task_runner_weak_;
};

template <typename UserModule>
void LinkModuleImpl<UserModule>::Initialize() {
  module_->Initialize(task_runner_weak_, spec_.arguments());
}

template <typename UserModule>
void LinkModuleImpl<UserModule>::Process() {
  module_->Process();
}

template <typename UserModule>
void LinkModuleImpl<UserModule>::Terminate() {
  module_->Terminate();
}

template <typename UserModule>
bool LinkModuleImpl<UserModule>::IsRunning() const {
  return module_->IsRunning();
}

LinkModulePtr LinkModule::CreateModule(
  std::weak_ptr<base::TaskRunner> task_runner_weak,
  ModuleClient* client,
  const Specification& spec) {
  LinkModuleHandle* module_handle = new LinkModuleHandle();
  if (!module_handle->Open(spec.path(), RTLD_LAZY | RTLD_GLOBAL)) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name() << " load fail";
    return nullptr;
  }

  auto* test = ModuleRegister::GetInstance();
  const AbstractModlueFactory<UserModuleBase>* factory =
    test->GetModuleFactory<UserModuleBase>(
      spec.class_name());

  if (!factory) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name() << " can not find factory";
    return nullptr;
  }

  LinkModulePtr module_impl(
    new LinkModuleImpl<UserModuleBase>(
      module_handle,
      factory->CreateModuleObject(spec.module_name(), client),
      task_runner_weak,
      spec),
      &ModuleDeleter);
  return module_impl;
}

}  // namespace module
}  // namespace nlink
