/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_SPECIFICATION_H_
#define LINK_MODULE_SPECIFICATION_H_

#include <string>

#include "link/base/json_wrapper.h"

namespace link {
namespace module {

class Specification {
 public:
  Specification() = default;
  Specification(const Specification& spec);
  Specification(Specification&& spec);

  ~Specification() = default;

  void ParseFromStr(const std::string& spec_json_str);

  const std::string name() const;
  const std::string path() const;
  const std::string class_name() const;

  template<typename T>
  const T args(const std::string& key) const {
    return args_.get<T>(key);
  }

  template<typename T>
  const T configure(const std::string& key) const {
    return configure_.get<T>(key);
  }

  Specification& operator=(const Specification& spec);
  Specification& operator=(Specification&& spec);

 private:
  bool CheckKeyExist(const base::JsonWrapper& json, const std::string& key);

  std::string name_;
  std::string path_;
  std::string class_name_;
  base::JsonWrapper args_;
  base::JsonWrapper configure_;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_SPECIFICATION_H_
