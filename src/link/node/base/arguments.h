/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NODE_BASE_ARGUMENTS_H_
#define LINK_NODE_BASE_ARGUMENTS_H_

#include <string>
#include <vector>

#include "link/module/base/specification.h"

namespace nlink {
namespace node {

class Arguments {
 public:
  Arguments();
  ~Arguments();

  bool ParseFromeProcessArguments(int32_t argc, char* argv[]);

  const std::string config_path() const;
  const std::string node_name() const;
  const std::string log_level() const;
  std::vector<module::Specification> module_specs() const;

 private:
  bool ParseFromeConfigureFile(const std::string&  config_file_path);

  bool empty_;

  std::string config_path_;
  std::string node_name_;
  std::string log_level_;
  std::vector<module::Specification> module_specs_;
};

}  // namespace node
}  // namespace nlink

#endif  // LINK_NODE_BASE_ARGUMENTS_H_
