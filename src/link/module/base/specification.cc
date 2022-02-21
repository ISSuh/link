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
  ParseFromJson(spec_json);
}

void Specification::ParseFromJson(const base::JsonWrapper& spec_json) {
  if (!spec_json.hasKey(kModuleNameKey) ||
      !spec_json.hasKey(kModulePathKey) ||
      !spec_json.hasKey(kModuleKey)) {
    return;
  }

  base::JsonWrapper spec_module_json(spec_json[kModuleKey].dump());
  if (!spec_module_json.hasKey(kClassNameKey) ||
      !spec_module_json.hasKey(kArgumentsKey) ||
      !spec_module_json.hasKey(kConfigureKey)) {
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

}  // namespace module
}  // namespace link
