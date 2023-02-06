/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/storage/database/adaptor/sqlite3/sqlite3_adaptor.h"

#include <sstream>

#include "link/base/logging.h"

namespace nlink {
namespace storage {

Sqlite3::Sqlite3()
  : db_handle_(nullptr) {
}

Sqlite3::~Sqlite3() {
}

bool Sqlite3::Open(const std::string& path) {
  if (nullptr != db_handle_) {
    Close();
  }

  int32_t res = -1;

  res = ::sqlite3_initialize();
  if (SQLITE_OK != res) {
      LOG(LogLevel::ERROR) << __func__
         << " - initailize fail. " << ErrorMessage();
      return false;
  }

  res =::sqlite3_open_v2(path.c_str(), &db_handle_,
          SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  if (SQLITE_OK != res) {
      LOG(LogLevel::ERROR) << __func__
         << " - Cannot open database. " << ErrorMessage();
      Close();
      return false;
  }
  return true;
}

bool Sqlite3::Excute(const std::string& sql) {
  int32_t res = -1;
  sqlite3_stmt* stmt = nullptr;

  res = sqlite3_prepare_v2(db_handle_, sql.c_str(), -1, &stmt, nullptr);
  if (SQLITE_OK != res) {
    LOG(LogLevel::ERROR) << __func__
        << " - SQL compiled fail. " << ErrorMessage();

    sqlite3_finalize(stmt);
    return false;
  }

  int32_t ret_code = 0;
  std::stringstream stream;
  while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
      printf("TEST: ID = %d\n", sqlite3_column_int(stmt, 0));
  }

  if(ret_code != SQLITE_DONE) {
      printf("ret_code = %d\n", ret_code);
  }

  sqlite3_finalize(stmt);
  return false;
}

void Sqlite3::Close() {
  int32_t res = -1;

  res = sqlite3_close(db_handle_);
  if (SQLITE_OK != res) {
      LOG(LogLevel::ERROR) << __func__
         << " - Cannot close database. " << ErrorMessage();
  }

  res = ::sqlite3_shutdown();
  if (SQLITE_OK != res) {
      LOG(LogLevel::ERROR) << __func__
         << " - Cannot shutdown db. " << ErrorMessage();
  }
}

std::string Sqlite3::ErrorMessage() {
  return nullptr == db_handle_ ? "" : ::sqlite3_errmsg(db_handle_);
}

}  // namespace storage
}  // namespace nlink
