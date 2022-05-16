/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/json_wrapper.h"

#include "link/base/logging.h"

namespace nlink {
namespace base {

bool CheckKeyExist(const base::Json& json, const std::string& key) {
  if (json.find(key) == json.end()) {
    LOG(INFO) << " Not fount key : " << key;
    return false;
  }
  return true;
}

}  // namespace base
}  // namespace nlink
