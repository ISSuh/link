/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/module/specification.h"

#include "base/logging.h"

namespace link {
namespace module {



Specification::Specification(const Specification& spec)
  : name_(), path_(), claas_name_(),
    args_(), configure_() {
  name_ = spec.name_;
  path_ = spec.path_;
  claas_name_ = spec.claas_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
}

Specification::Specification(Specification&& spec)
  : name_(), path_(), claas_name_(),
    args_(), configure_() {
  name_ = spec.name_;
  path_ = spec.path_;
  claas_name_ = spec.claas_name_;
  args_ = spec.args_;
  configure_ = spec.configure_;
}

void Specification::ParseFromeStr(const std::string& json_str) {
  base::JsonWrapper spec_json(json_str);

  name_ = SetSpecStrValue(spec_json, kModuleNameKey);
  path_ = SetSpecStrValue(spec_json, kModulePathKey);
  claas_name_ = SetSpecStrValue(spec_json, kClassNameKey);
  args_ = SetSpecSubJsonValue(spec_json, kArgumentsKey);
  configure_ = SetSpecSubJsonValue(spec_json, kConfigureKey);
}

}  // namespace module
}  // namespace link
