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

const char* kModuleNameKey = "module_name";
const char* kModulePathKey = "module_path";
const char* kModuleKey = "module";
const char* kClassNameKey = "class_name";
const char* kArgumentsKey = "args";
const char* kConfigureKey = "configure";

bool CheckKeyExist(
  const base::JsonWrapper& json, const std::string& key) {
  if (!json.hasKey(key)) {
    LOG(WARN) << __func__ << " - invalid key : " << key;
    return false;
  }
  return true;
}

void ModuleDeleter(Module* module) {
  LOG(INFO) << __func__;
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

  virtual void RunModule();

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
  return spec_.name;
}

const std::string Module::path() const {
  return spec_.path;
}

const std::string Module::class_name() const {
  return spec_.class_name;
}

Module::Specification::Specification(const Specification& spec)
  : name(spec.name), path(spec.path), class_name(spec.class_name),
    args(spec.args.dump()), configure(spec.configure.dump()) {
}

void Module::Specification::ParseFromStr(const std::string& json_str) {
  base::JsonWrapper spec_json(json_str);

  if (!CheckKeyExist(spec_json, kModuleNameKey) ||
      !CheckKeyExist(spec_json, kModulePathKey) ||
      !CheckKeyExist(spec_json, kModuleKey)) {
    return;
  }

  base::JsonWrapper spec_module_json(spec_json[kModuleKey].dump());
  if (!CheckKeyExist(spec_module_json, kClassNameKey) ||
      !CheckKeyExist(spec_module_json, kArgumentsKey) ||
      !CheckKeyExist(spec_module_json, kConfigureKey)) {
    return;
  }

  name = spec_json.getString(kModuleNameKey);
  path = spec_json.getString(kModulePathKey);
  class_name = spec_module_json.getString(kClassNameKey);
  args = spec_module_json[kArgumentsKey];
  configure = spec_module_json[kConfigureKey];
}

ModulePtr Module::CreateModule(const Specification& spec) {
  ModuleHandle* module_handle = new ModuleHandle();
  if (!module_handle->Open(spec.path, RTLD_LAZY | RTLD_GLOBAL)) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name << " load fail";
    return nullptr;
  }

  const AbstractModlueFactory<UserModuleBase>* factory =
    ModuleRegister::GetModuleFactory<UserModuleBase>(spec.class_name);
  if (!factory) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name << " can not find factory";
    return nullptr;
  }

  ModulePtr module_impl(
    new ModuleImpl<UserModuleBase>(
      module_handle, factory->CreateModuleObject(), spec),
      &ModuleDeleter);
  return module_impl;
}

template <typename UserModuleBaseType>
void ModuleImpl<UserModuleBaseType>::RunModule() {
  module_->Initialize();
  module_->Process();
  module_->Shutdown();
}

}  // namespace module
}  // namespace link
