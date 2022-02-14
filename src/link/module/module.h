/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_MODULE_H_
#define LINK_MODULE_MODULE_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/json_wrapper.h"

namespace link {
namespace module {

class Module {
 public:
  struct Specification {
    Specification() = default;
    virtual ~Specification() = default;

    void ParseFromStr(const std::string& json_str);

    std::string name;
    std::string path;
    std::string class_name;
    base::JsonWrapper args;
    base::JsonWrapper configure;
  };

  static std::unique_ptr<Module> CreateModule(const Specification& spec);

  const std::string name() const;
  const std::string class_name() const;
  const std::string path() const;

 protected:
  explicit Module(const Specification& spec);
  virtual ~Module() = default;

  const Specification spec_;

  DISAALOW_COPY_AND_ASSIGN(Module)
};


}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_MODULE_H_
