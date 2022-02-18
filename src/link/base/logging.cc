/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/logging.h"

#include "link/base/time.h"

namespace link {

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
  if (CanPrintLog()) {
    buffer_ << "\033[" << SelectLogColor(level) << "m"
            << "[" << DateToStr() << "][" << LogLevelToStr(level) << "] : ";
  }
}

LOG::~LOG() {
  if (CanPrintLog()) {
    buffer_ << std::endl;
    std::cerr << buffer_.str();
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
  case LogLevel::WARN:
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
  case LogLevel::WARN:
    label = "WARN";
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

}  // namespace link
