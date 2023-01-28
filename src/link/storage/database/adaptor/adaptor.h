/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_DATABASE_ADAPTOR_ADAPTOR_H_
#define LINK_STORAGE_DATABASE_ADAPTOR_ADAPTOR_H_

#include <string>

namespace nlink {
namespace storage {

class Adaptor {
 public:
  virtual ~Adaptor() = default;

  virtual bool Open(const std::string& path) = 0;
  virtual bool Excute(const std::string& sql) = 0;
  virtual void Close() = 0;
};

}  // namespace storage
}  // namespace nlink

#endif  // #define LINK_STORAGE_DATABASE_ADAPTOR_ADAPTOR_H_
