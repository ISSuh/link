/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/base/specification.h"

#include <utility>

#include "link/base/logging.h"

namespace link {
namespace module {

const char* kModuleNameKey = "module_name";
const char* kModulePathKey = "module_path";
const char* kModuleKey = "module";
const char* kClassNameKey = "class_name";
const char* kArgumentsKey = "args";
const char* kConfigureKey = "configure";

Specification::Specification(const Specification& spec)
  : name_(), path_(), class_name_(),
    args_(), configure_() {
  name_ = spec.name_;
  path_ = spec.path_;
  class_name_ = spec.class_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
}

Specification::Specification(Specification&& spec)
  : name_(), path_(), class_name_(),
    args_(), configure_() {
  name_ = spec.name_;
  path_ = spec.path_;
  class_name_ = spec.class_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
}

void Specification::ParseFromStr(const std::string& spec_json_str) {
  base::JsonWrapper spec_json(spec_json_str);

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

  name_ = spec_json.getString(kModuleNameKey);
  path_ = spec_json.getString(kModulePathKey);
  class_name_ = spec_module_json.getString(kClassNameKey);
  args_ = spec_module_json[kArgumentsKey];
  configure_ = spec_module_json[kConfigureKey];
}

const std::string Specification::name() const {
  return name_;
}
const std::string Specification::path() const {
  return path_;
}
const std::string Specification::class_name() const {
  return class_name_;
}

Specification& Specification::operator=(const Specification& spec) {
  if (&spec == this) {
    return *this;
  }

  name_ = spec.name_;
  path_ = spec.path_;
  class_name_ = spec.class_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
  return *this;
}

Specification& Specification::operator=(Specification&& spec) {
  if (&spec == this) {
    return *this;
  }

  name_ = spec.name_;
  path_ = spec.path_;
  class_name_ = spec.class_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
  return *this;
}

bool Specification::CheckKeyExist(
  const base::JsonWrapper& json, const std::string& key) {
  if (!json.hasKey(key)) {
    LOG(WARN) << __func__ << " - invalid key : " << key;
    return false;
  }
  return true;
}

}  // namespace module
}  // namespace link
