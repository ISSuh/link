 /**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "storage_module.h"

#include <link/base/logging.h>

using namespace nlink;

void SampleStorageModule::Init() {
  LOG(INFO) << "SampleModule::init";
}

void SampleStorageModule::Run() {
  LOG(INFO) << "SampleModule::Run";
}

void SampleStorageModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}
