/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_SPECIFICATION_H_
#define LINK_MODULE_SPECIFICATION_H_

#include <string>

#include "link/base/json_wrapper.h"

namespace nlink {
namespace module {

class Specification {
 public:
  Specification() = default;
  Specification(const Specification& spec);
  Specification(Specification&& spec);

  ~Specification() = default;

  bool ParseFromStr(const std::string& spec_json_str);
  bool ParseFromJson(const base::Json& spec_json);

  const std::string module_name() const;
  const std::string path() const;
  const std::string class_name() const;
  const base::Json arguments() const;
  const base::Json configure() const;

  Specification& operator=(const Specification& spec);
  Specification& operator=(Specification&& spec);

 private:
  std::string module_name_;
  std::string path_;
  std::string class_name_;
  base::Json arguments_;
  base::Json configure_;
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_SPECIFICATION_H_
