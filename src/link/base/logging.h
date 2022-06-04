/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_LOGGING_H_
#define LINK_BASE_LOGGING_H_

#include <iostream>
#include <string>
#include <sstream>
#include <memory>

namespace nlink {

enum LogLevel {
  ERROR = 0X01,
  WARNING = 0X02,
  INFO = 0X03,
  DEBUG = 0X04,
  TRACE = 0X05,
};

void SetLogLevel(LogLevel level);
LogLevel GetLogLevel();

class LOG {
 public:
  LOG() = default;
  explicit LOG(LogLevel level);
  ~LOG();

  template <typename T>
  LOG& operator<<(const T& message);

 private:
  enum LogColorCode {
    RED      = 31,
    GREEN    = 32,
    YELLOW   = 33,
    BLUE     = 34,
    BLACK    = 39,
  };

  bool CanPrintLog();

  LogColorCode SelectLogColor(LogLevel level);
  std::string LogLevelToStr(LogLevel level);
  std::string DateToStr();

  LogLevel log_level_;
  std::ostringstream buffer_;
};

template <typename T>
LOG& LOG::operator<<(const T& message) {
  if (CanPrintLog()) {
    buffer_ << message;
  }
  return *this;
}

namespace base {

class LoggerImpl;

class Logger {
 public:
  explicit Logger(const std::string& name);
  ~Logger();

 private:
  std::unique_ptr<LoggerImpl> logger_impl_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_LOGGING_H_
