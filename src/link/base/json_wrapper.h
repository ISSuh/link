/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_JSON_WRAPPER_H_
#define LINK_BASE_JSON_WRAPPER_H_

#include <string>

#include "link/third_party/json/json.hpp"

namespace nlink {
namespace base {

using Json = nlohmann::json;

bool CheckKeyExist(const base::Json& json, const std::string& key);

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_JSON_WRAPPER_H_
