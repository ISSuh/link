/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_LOGGER_H_
#define LINK_BASE_LOGGER_H_

#include <string>
#include <map>
#include <memory>
#include <atomic>
#include <mutex>

#include "link/base/logging.h"
#include "link/base/task_manager.h"

namespace nlink {
namespace base {

class Logger {
 public:
  virtual void PrintLog(LogLevel level, const std::string& log) = 0;
};

class LoggerManager {
 public:
  static LoggerManager* Instance();

  void SetTaskManager(std::shared_ptr<base::TaskManager>);

  std::shared_ptr<Logger> CreateLogger(const std::string name, LogLevel level);
  std::shared_ptr<Logger> FineLoggerFromThreadId(uint64_t thread_id);

 private:
  LoggerManager();
  ~LoggerManager();

  void UpdateThreadId();

  static std::atomic<LoggerManager*> instance_;
  static std::mutex mutex_;

  using ThreadId = uint64_t;
  std::map<ThreadId, std::shared_ptr<Logger>> logger_map_;
  std::shared_ptr<Logger> default_logger_;

  std::shared_ptr<base::TaskManager> task_manager_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_LOGGER_H_
