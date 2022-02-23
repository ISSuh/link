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
const char* kArgumentsKey = "arguments";
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

bool Specification::ParseFromStr(const std::string& spec_json_str) {
  base::Json spec_json(spec_json_str);
  return ParseFromJson(spec_json);
}

bool Specification::ParseFromJson(const base::Json& spec_json) {
  if (!base::CheckKeyExist(spec_json, kModuleNameKey) ||
      !base::CheckKeyExist(spec_json, kModulePathKey) ||
      !base::CheckKeyExist(spec_json, kModuleKey)) {
    return false;
  }

  base::Json module_json = spec_json[kModuleKey];
  if (!base::CheckKeyExist(module_json, kClassNameKey) ||
      !base::CheckKeyExist(module_json, kArgumentsKey) ||
      !base::CheckKeyExist(module_json, kConfigureKey)) {
    return false;
  }

  name_ = spec_json[kModuleNameKey].get<std::string>();
  path_ = spec_json[kModulePathKey].get<std::string>();
  class_name_ = module_json[kClassNameKey].get<std::string>();
  args_ = module_json[kArgumentsKey];
  configure_ = module_json[kConfigureKey];

  return true;
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

}  // namespace module
}  // namespace link
