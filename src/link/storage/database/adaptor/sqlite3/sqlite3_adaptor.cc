/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/storage/database/adaptor/sqlite3/sqlite3_adaptor.h"

#include "link/base/logging.h"

namespace nlink {
namespace storage {

Sqlite3Adaptor::Sqlite3Adaptor()
  : db_handle_(nullptr) {
}

Sqlite3Adaptor::~Sqlite3Adaptor() {
}

bool Sqlite3Adaptor::Open(const std::string& path) {
  if (nullptr != db_handle_) {
    Close();
  }

  sqlite3* handle = db_handle_.get();
  int32_t res = sqlite3_open(path.c_str(), &handle);
  if (SQLITE_OK != res) {
      LOG(LogLevel::ERROR) << __func__
         << " - Cannot open database: " << sqlite3_errmsg(handle);
      Close();
      return false;
  }
}

bool Sqlite3Adaptor::Excute(const std::string& sql) {

}
bool Sqlite3Adaptor::Close() {
  sqlite3_close(db_handle_.get());
  db_handle_.reset();
}

}  // namespace storage
}  // namespace nlink
