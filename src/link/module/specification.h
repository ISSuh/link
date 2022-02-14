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

// class Specification {
//  public:
//   Specification(const Specification& spec);
//   Specification(Specification&& spec);

//   void ParseFromeStr(const std::string& json_str);

//   const std::string name() const;
//   const std::string path() const;
//   const std::string class_name() const;

//   template<typename T>
//   const T args(const std::string& key) const {
//     return args_.get<T>(key);
//   }

//   template<typename T>
//   const T configure(const std::string& key) const {
//     return configure_.get<T>(key);
//   }


//   Specification&& operator=(Specification&& spec) {
//     if (&key == this) {
//       return *this;
//     }

//     name_ = spec.name_;
//     path_ = spec.path_;
//     claas_name_ = spec.claas_name_;
//     args_ = spec.args_;
//     configure_ = spec.configure_;
//     return *this;
//   }

//  private:
//   std::string name_;
//   std::string path_;
//   std::string claas_name_;
//   base::JsonWrapper args_;
//   base::JsonWrapper configure_;
// };

}  // namespace module
}  // namespace link

#endif  // LINK_MODULE_SPECIFICATION_H_
