/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/logger.h"

#include <iostream>
#include <string>
#include <thread>

#include "link/base/time.h"
#include "link/base/thread.h"

namespace nlink {
namespace base {

std::atomic<LoggerManager*> LoggerManager::instance_{nullptr};
std::mutex LoggerManager::mutex_;

enum LogColorCode {
  RED      = 31,
  GREEN    = 32,
  YELLOW   = 33,
  BLUE     = 34,
  BLACK    = 39,
};

inline std::string LogLevelToStr(const LogLevel level) {
  switch (level) {
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::TRACE:
      return "TRACE";
    }
  return "";
}

LogColorCode SelectLogColor(LogLevel level) {
  LogColorCode color;
  switch (level) {
    case LogLevel::ERROR:
      color = LogColorCode::RED;
      break;
    case LogLevel::WARNING:
      color = LogColorCode::YELLOW;
      break;
    case LogLevel::INFO:
      color = LogColorCode::GREEN;
      break;
    case LogLevel::DEBUG:
      color = LogColorCode::BLUE;
      break;
    case LogLevel::TRACE:
      color = LogColorCode::BLACK;
      break;
  }
  return color;
}
class LoggerImpl : public Logger {
 public:
  LoggerImpl(const std::string& name, LogLevel level);
  virtual ~LoggerImpl();

  void PrintLog(LogLevel level, const std::string& log) override;

 private:
  std::string name_;
  LogLevel log_level_;
};

LoggerImpl::LoggerImpl(const std::string& name, LogLevel level)
  : name_(name), log_level_(level) {
}

LoggerImpl::~LoggerImpl() = default;

void LoggerImpl::PrintLog(LogLevel level, const std::string& log) {
  if (log_level_ < level) {
    return;
  }

  std::stringstream ss;
  ss << "\033[" << SelectLogColor(level) << "m";
  ss << "[" << Time::CurrentTimeToDateStr() << "]";
  ss << "[" << std::this_thread::get_id() <<  "]";
  ss << "[" << LogLevelToStr(level) << "]";
  ss << "<" << name_ << "> ";
  ss <<  log;

  std::cout << ss.str() << std::endl;
}

LoggerManager* LoggerManager::Instance() {
  if (nullptr == instance_) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr == instance_) {
      instance_ = new LoggerManager();
    }
  }
  return instance_;
}

LoggerManager::LoggerManager()
  : default_logger_(std::make_shared<LoggerImpl>("main", LogLevel::INFO)) {
}

LoggerManager::~LoggerManager() {
}

void LoggerManager::SetTaskManager(
  std::shared_ptr<base::TaskManager> task_manager) {
  task_manager_ = task_manager;
}

std::shared_ptr<Logger> LoggerManager::CreateLogger(
  const std::string name, LogLevel level) {
  std::shared_ptr<Logger> logger = std::make_shared<LoggerImpl>(name, level);

  TaskManager::TaskThreadIdByLabel id_by_label = task_manager_->TaskThreadIds();
  for (const auto& item : id_by_label) {
    const std::string label = item.first;
    const uint64_t id = item.second;
    if (name == label) {
      logger_map_[id] = logger;
    }
  }

  return logger;
}

std::shared_ptr<Logger> LoggerManager::FineLoggerFromThreadId(
  uint64_t thread_id) {
  if (logger_map_.find(thread_id) == logger_map_.end()) {
    return default_logger_;
  }
  return logger_map_[thread_id];
}

void LoggerManager::UpdateThreadId() {
  TaskManager::TaskThreadIdByLabel id_by_label = task_manager_->TaskThreadIds();
  for (const auto& item : id_by_label) {
    const std::string name = item.first;
    const uint64_t id = item.second;

    std::shared_ptr<Logger> logger =
      std::make_shared<LoggerImpl>(name, LogLevel::INFO);
    logger_map_[id] = logger;
  }
}

}  // namespace base
}  // namespace nlink
