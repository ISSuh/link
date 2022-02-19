/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/arguments.h"

namespace link {
namespace base {

Arguments::Arguments(int32_t argc, char* argv[])
  : empty_(false) {
  
  if (argc < 2) {
    empty_ = true;
    return;
  }

  ParseModuleConfigPath(argv[1]);
} 

Arguments::~Arguments() {
}

bool Arguments::IsEmpty() const {
  return empty_;
}

const std::string Arguments::module_config_path() const {
  return module_config_path_;
}

void Arguments::ParseModuleConfigPath(const char* module_config_path) {
  module_config_path_ = module_config_path;
}

}  // namespace base
}  // namespace link
