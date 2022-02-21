/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/arguments.h"

#include <fstream>
#include <streambuf>

#include "link/base/json_wrapper.h"
#include "link/base/logging.h"

namespace link {
namespace node {

int32_t kMinArgsNums = 1;
int8_t kConfigFileIndex = 1;

const char* kNodeNameKey = "node_name";
const char* kLogLevelKey = "log_level";
const char* kModulesKey = "modules";

Arguments::Arguments()
  : config_path_(),
    node_name_(),
    log_level_(),
    module_specs_() {
}

Arguments::~Arguments() {
}

bool Arguments::ParseFromeProcessArguments(int32_t argc, char* argv[]) {
  if (argc < kMinArgsNums) {
    LOG(ERROR) << " Empty arguments";
    return false;
  }

  const std::string config_file_path = argv[kConfigFileIndex];
  return ParseFromeConfigureFile(config_file_path);
}

const std::string Arguments::config_path() const {
  return config_path_;
}

const std::string Arguments::node_name() const {
  return node_name_;
}

const std::string Arguments::log_level() const {
  return log_level_;
}

std::vector<module::Specification> Arguments::module_specs() const {
  return module_specs_;
}

bool Arguments::ParseFromeConfigureFile(const std::string& config_file_path) {
  std::ifstream config_file(config_file_path);

  if (!config_file.good()) {
    LOG(ERROR) << " Invalid configure file path : " << config_file_path;
    return false;
  }

  config_path_ = config_file_path;

  std::string config_str((std::istream_iterator<char>(config_file)),
                      std::istream_iterator<char>());

  base::JsonWrapper config_json(config_str);

  if (!config_json.hasKey(kNodeNameKey) ||
      config_json.getString(kNodeNameKey).empty()) {
    LOG(ERROR) << " Invalid node name";
    return false;
  }

  node_name_ = config_json.getString(kNodeNameKey);

  if (!config_json.hasKey(kModulesKey) ||
      config_json.getString(kModulesKey).empty()) {
    LOG(ERROR) << " Invalid node name";
    return false;
  }

  base::JsonWrapper modules_json(config_json[kModulesKey].dump());
  
  node_name_ = config_json.getString(kModulesKey);

}

}  // namespace node
}  // namespace link
