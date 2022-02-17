/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_JSON_WRAPPER_H_
#define LINK_BASE_JSON_WRAPPER_H_

#include <string>

#include "link/third_party/json/json.hpp"

namespace link {
namespace base {

using json = nlohmann::json;

class JsonWrapper {
 public:
  JsonWrapper() : json_obj() {}
  explicit JsonWrapper(const char* json_str);
  explicit JsonWrapper(const std::string& json_str);
  JsonWrapper(const JsonWrapper& json);

  virtual ~JsonWrapper() = default;

  void parse(const char* json_str);
  void parse(const std::string& json_str);

  const std::string dump() const;

  void clear();
  uint32_t size() const;

  bool empty() const;
  bool hasKey(const std::string& key) const;
  // bool hasKey(const char* key);
  bool hasIndex(int index);

  template<typename T>
  T get(const std::string& key) {
    return json_obj[key];
  }

  bool getBool(const std::string& key) const;
  int32_t getInt(const std::string& key) const;
  uint32_t getUint(const std::string& key) const;
  const std::string getString(const std::string& key) const;

  JsonWrapper& operator=(const JsonWrapper& rhs) {
    if (&rhs != this) {
      return *this;
    }

    clear();
    json_obj = rhs.json_obj;

    return *this;
  }

  JsonWrapper operator[](const char* key) const {
    return JsonWrapper(json_obj[key].dump());
  }

  JsonWrapper operator[](const std::string& key) const {
    return JsonWrapper(json_obj[key].dump());
  }

  JsonWrapper operator[](int index) const {
    return JsonWrapper(json_obj.at(index).dump());
  }

 private:
  json json_obj;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_JSON_WRAPPER_H_
