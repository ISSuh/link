/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/json_wrapper.h"

namespace link {
namespace base {

using json = nlohmann::json;

JsonWrapper::JsonWrapper(const char* json_str) {
  json_obj = json::parse(json_str);
}

JsonWrapper::JsonWrapper(const std::string& json_str)
  : JsonWrapper(json_str.c_str()) {}

JsonWrapper::JsonWrapper(const JsonWrapper& json) {
  clear();
  json_obj = json.json_obj;
}

void JsonWrapper::parse(const char* json_str) {
  clear();
  json_obj = json::parse(json_str);
}

void JsonWrapper::parse(const std::string& json_str) {
  parse(json_str.c_str())
}

const std::string JsonWrapper::dump() const {
  return json_obj.dump();
}

void JsonWrapper::clear() {
  json_obj.clear();
}

uint32_t JsonWrapper::size() const {
  return json_obj.size();
}

bool JsonWrapper::empty() const {
  return json_obj.empty();
}

bool JsonWrapper::hasKey(const std::string& key) {
  return json_obj.find(key) != json_obj.end();
}

bool JsonWrapper::hasKey(const char* key) {
  return json_obj.find(key) != json_obj.end();
}

bool JsonWrapper::hasIndex(int index) {
  return json_obj[index].empty();
}

bool JsonWrapper::getBool(const std::string& key) const {
  return json_obj[key];
}

int32_t JsonWrapper::getInt(const std::string& key) const {
  return json_obj[key];
}

uint32_t JsonWrapper::getUint(const std::string& key) const  {
  return json_obj[key];
}

const std::string JsonWrapper::getString(const std::string& key) const {
  return json_obj[key];
}

}  // namespace base
}  // namespace link
