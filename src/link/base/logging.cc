/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/logging.h"

#include <string>
#include <memory>
#include <thread>

#include "link/base/thread.h"
#include "link/base/logger.h"

namespace nlink {

LOG::LOG(LogLevel level) : log_level_(level) {
}

LOG::~LOG() {
  uint64_t current_thread_id = base::HashedCurrentThreadId();
  std::shared_ptr<base::Logger> logger =
    base::LoggerManager::Instance()->FineLoggerFromThreadId(current_thread_id);

  logger->PrintLog(log_level_, buffer_.str());
}

}  // namespace nlink
