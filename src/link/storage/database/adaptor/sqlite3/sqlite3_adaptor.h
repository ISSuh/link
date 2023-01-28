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


class People {
 public:
 private:
  PK<uint32_t> id;
  

  int32_t id;
  std::string name;
}

class Sqlite3 : public Adaptor {
 public:
  Sqlite3();
  virtual ~Sqlite3();

  bool Open(const std::string& path) override;
  bool Excute(const std::string& sql) override;
  void Close() override;

 private:
  std::string ErrorMessage();

  sqlite3* db_handle_;
};

}  // namespace storage
}  // namespace nlink

#endif  // #define LINK_STORAGE_DATABASE_ADAPTOR_SQLITE3_SQLITE3_ADAPTOR_H_
