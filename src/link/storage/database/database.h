/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_DATABASE_STORAGE_H_
#define LINK_STORAGE_DATABASE_STORAGE_H_

#include <string>
#include <memory>

#include "link/storage/database/adaptor/adaptor.h"

namespace nlink {
namespace storage {

class Database {
 public:
  explicit Database(std::unique_ptr<Adaptor> adaptor);
  ~Database();

  bool Connect(const std::string& path);
  void Disconnect();
  void Excute(const std::string & sql);

 private:
  std::unique_ptr<Adaptor> adaptor_;
};

}  // namespace storage
}  // namespace nlink

#endif  // LINK_STORAGE_DATABASE_STORAGE_H_
