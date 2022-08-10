/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/logger.h"


#include <string>

#include "link/base/time.h"
#include "link/base/thread.h"
#include "link/third_party/spdlog/spdlog.h"
#include "link/third_party/spdlog/sinks/stdout_color_sinks.h"
#include "link/third_party/spdlog/sinks/basic_file_sink.h"

namespace nlink {
namespace base {

std::atomic<LoggerManager*> LoggerManager::instance_{nullptr};
std::mutex LoggerManager::mutex_;

class LoggerImpl : public Logger {
 public:
  explicit LoggerImpl(const std::string& name);
  virtual ~LoggerImpl();

  void PrintLog(LogLevel level, const std::string& log) override;

 private:
  std::string name_;

  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
  std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink_;
};

LoggerImpl::LoggerImpl(const std::string& name)
  : name_(name) {
}

LoggerImpl::~LoggerImpl() = default;

void LoggerImpl::PrintLog(LogLevel level, const std::string& log) {
  switch (level) {
    case LogLevel::ERROR:
      SPDLOG_ERROR("<{}> {}", name_, log);
      break;
    case LogLevel::WARNING:
      SPDLOG_WARN("<{}> {}", name_, log);
      break;
    case LogLevel::INFO:
      SPDLOG_INFO("<{}> {}", name_, log);
      break;
    case LogLevel::DEBUG:
      SPDLOG_DEBUG("<{}> {}", name_, log);
      break;
    case LogLevel::TRACE:
      SPDLOG_TRACE("<{}> {}", name_, log);
      break;
  }
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
  : default_logger_(std::make_shared<LoggerImpl>("main")) {
  spdlog::set_pattern("[%H:%M:%S:%e:%f][%P:%t][%^%L%$] %v");
}

LoggerManager::~LoggerManager() {
}

void LoggerManager::SetTaskManager(
  std::shared_ptr<base::TaskManager> task_manager) {
  task_manager_ = task_manager;
}

std::shared_ptr<Logger> LoggerManager::CreateLogger(const std::string name) {
  std::shared_ptr<Logger> logger = std::make_shared<LoggerImpl>(name);

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

    std::shared_ptr<Logger> logger = std::make_shared<LoggerImpl>(name);
    logger_map_[id] = logger;
  }
}

}  // namespace base
}  // namespace nlink
