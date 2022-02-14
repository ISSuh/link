/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/module.h"

#include <dlfcn.h>

#include <string>
#include <memory>
#include <utility>

#include "link/module/user_module.h"
#include "link/module/loader/module_factory.h"
#include "base/logging.h"

namespace link {
namespace module {

const char* kModuleNameKey = "module_name";
const char* kModulePathKey = "module_path";
const char* kClassNameKey = "class_name";
const char* kArgumentsKey = "args";
const char* kConfigureKey = "configure";

std::string SetSpecStrValue(
  const base::JsonWrapper& json, const std::string& key) {
  if (!json.hasKey(key)) {
    LOG(WARN) << __func__ << " - invalid key : " << key
                          << ", return empty";
    return std::string();
  }
  return json.getString(key);
}

base::JsonWrapper SetSpecSubJsonValue(
  const base::JsonWrapper& json, const std::string& key) {
  if (!json.hasKey(key)) {
    LOG(WARN) << __func__ << " - invalid key : " << key
                          << ", return empty";
    return base::JsonWrapper();
  }
  return json[key];
}

void Module::Specification::ParseFromStr(const std::string& json_str) {
  base::JsonWrapper spec_json(json_str);

  name = SetSpecStrValue(spec_json, kModuleNameKey);
  path = SetSpecStrValue(spec_json, kModulePathKey);
  class_name = SetSpecStrValue(spec_json, kClassNameKey);
  args = SetSpecSubJsonValue(spec_json, kArgumentsKey);
  configure = SetSpecSubJsonValue(spec_json, kConfigureKey);
}

class ModuleHandle {
 public:
  ModuleHandle();
  ~ModuleHandle();

  bool Open(const std::string& path, int32_t flags);

 private:
  uint8_t* handle_;
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
    std::cerr << dlerror() << std::endl;
    return false;
  }
  return true;
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

template<typename UserModuleBase>
class ModuleImpl : public Module {
 public:
  Module(ModuleHandle* module_handle,
         UserModuleBase* user_module,
         const Specification& spec)
    : module_handle_(module_handle), module_(user_module), spec_(spec) {}

  virtual ~ModuleImpl() = default;

 private:
  std::unique_ptr<ModuleHandle> module_handle_;
  std::unique_ptr<UserModuleBase> module_;
};

std::unique_ptr<Module> Module::CreateModule(const Specification& spec) {
  ModuleHandle* module_handle = new ModuleHandle();
  if (!module_handle->Open(spec.path, RTLD_LAZY | RTLD_GLOBAL)) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name << " load fail";
    return nullptr;
  }

  const AbstractModlueFactory<UserModule>* factory =
    ModuleRegister::GetModuleFactory<UserModule>(spec.class_name);
  if (!factory) {
    LOG(ERROR) << __func__ << " - "
               << spec.class_name << " can not find factory";
    return nullptr;
  }

  std::unique_ptr<Module> module_impl(
    new ModuleImpl<UserModule>(
      module_handle, factory->CreateModuleObject(), spec));
  return std::move(module_impl);
}

}  // namespace module
}  // namespace link
