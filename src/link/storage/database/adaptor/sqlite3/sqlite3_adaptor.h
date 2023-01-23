/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_DATABASE_ADAPTOR_SQLITE3_SQLITE3_ADAPTOR_H_
#define LINK_STORAGE_DATABASE_ADAPTOR_SQLITE3_SQLITE3_ADAPTOR_H_

#include <sqlite3.h>

#include <string>
#include <memory>

#include "link/storage/database/adaptor/adaptor.h"

namespace nlink {
namespace storage {

class Sqlite3Adaptor : public Adaptor {
 public:
  Sqlite3Adaptor();
  virtual ~Sqlite3Adaptor();

  bool Open(const std::string& path) override;
  bool Excute(const std::string& sql) override;
  bool Close() override;

 private:
  std::shared_ptr<sqlite3> db_handle_;
};

}  // namespace storage
}  // namespace nlink

#endif  // #define LINK_STORAGE_DATABASE_ADAPTOR_SQLITE3_SQLITE3_ADAPTOR_H_
