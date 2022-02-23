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

  bool ParseFromStr(const std::string& spec_json_str);
  bool ParseFromJson(const base::Json& spec_json);

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
  std::string name_;
  std::string path_;
  std::string class_name_;
  base::Json args_;
  base::Json configure_;
};

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_SPECIFICATION_H_
