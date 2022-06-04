/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/logging.h"

#include <string>

#include "link/base/time.h"
#include "link/third_party/spdlog/spdlog.h"
#include "link/third_party/spdlog/sinks/stdout_color_sinks.h"
#include "link/third_party/spdlog/sinks/basic_file_sink.h"

namespace nlink {

static bool active_logging = true;
static LogLevel system_log_level = LogLevel::INFO;

void ActiveLogging() {
  active_logging = true;
}

void DeactiveLogging() {
  active_logging = false;
}

void SetLogLevel(LogLevel level) {
  system_log_level = level;
}

LogLevel GetLogLevel() {
  return system_log_level;
}

LOG::LOG(LogLevel level) : log_level_(level) {
  spdlog::set_pattern("[%H:%M:%S:%e:%f][%P:%t][%^%L%$] %v");
}

LOG::~LOG() {
  switch (log_level_) {
    case LogLevel::ERROR:
      SPDLOG_ERROR("{}", buffer_.str());
      break;
    case LogLevel::WARNING:
      SPDLOG_WARN("{}", buffer_.str());
      break;
    case LogLevel::INFO:
      SPDLOG_INFO("{}", buffer_.str());
      break;
    case LogLevel::DEBUG:
      SPDLOG_DEBUG("{}", buffer_.str());
      break;
    case LogLevel::TRACE:
      SPDLOG_TRACE("{}", buffer_.str());
      break;
  }
}

bool LOG::CanPrintLog() {
  return active_logging && log_level_ <= system_log_level;
}

LOG::LogColorCode LOG::SelectLogColor(LogLevel level) {
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

std::string LOG::LogLevelToStr(LogLevel level) {
  std::string label("");
  switch (level) {
  case LogLevel::ERROR:
    label = "ERROR";
    break;
  case LogLevel::WARNING:
    label = "WARNING";
    break;
  case LogLevel::INFO:
    label = "INFO";
    break;
  case LogLevel::DEBUG:
    label = "DEBUG";
    break;
  case LogLevel::TRACE:
    label = "TRACE";
    break;
  }
  return label;
}

std::string LOG::DateToStr() {
  return base::Time::CurrentTimeToDateStr();
}

namespace base {

class LoggerImpl {
 public:
  explicit LoggerImpl(const std::string& name);
  ~LoggerImpl();

 private:
  std::string name_;

  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
  std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink_;

};

LoggerImpl::LoggerImpl(const std::string& name)
  : name_(name) {

}

LoggerImpl::~LoggerImpl() {
}

Logger::Logger(const std::string& name)
  : logger_impl_(new LoggerImpl(name)) {

}

Logger::~Logger() = default;

}  // namespace base
}  // namespace nlink
