/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/storage/database/database.h"

#include <utility>

namespace nlink {
namespace storage {

Database::Database(std::unique_ptr<Adaptor> adaptor)
  : adaptor_(std::move(adaptor)) {
}

Database::~Database() {
  adaptor_->Close();
  adaptor_.reset();
}

bool Database::Connect(const std::string& path) {
  if (path.empty()) {
    return false;
  }
  return adaptor_->Open(path);
}

void Database::Disconnect() {
  adaptor_->Close();
}

void Database::Excute(const std::string & sql) {
  adaptor_->Excute(sql);;
}

}  // namespace storage
}  // namespace nlink
