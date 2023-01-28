 /**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "storage_module.h"

#include <link/base/logging.h>
#include <link/storage/database/database_factory.h>
#include <link/storage/database/adaptor/sqlite3/sqlite3_adaptor.h>

using namespace nlink;

void SampleStorageModule::Init() {
  LOG(INFO) << "SampleModule::init";
  GetArgument("storage", &storage_path_);
}

void SampleStorageModule::Run() {
  LOG(INFO) << "SampleModule::Run";

  storage::DatabaseFactory db_factory;
  db_ = db_factory.CreateDatabase<storage::Sqlite3>();

  db_->Connect(storage_path_);
  db_->Disconnect();
}

void SampleStorageModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
