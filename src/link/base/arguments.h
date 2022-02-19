/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_ARGUMENTS_H_
#define LINK_BASE_ARGUMENTS_H_

#include <string>

#include "link/base/json_wrapper.h"

namespace link {
namespace base {

class Arguments {
 public:
  Arguments(int32_t argc, char* argv[]);
  ~Arguments();

  bool IsEmpty() const;
  const std::string module_config_path() const;

 private:
  void ParseModuleConfigPath(const char* module_config_path);

  bool empty_;
  std::string module_config_path_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_ARGUMENTS_H_
