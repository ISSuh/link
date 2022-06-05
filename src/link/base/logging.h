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

namespace nlink {

enum LogLevel {
  ERROR = 0X01,
  WARNING = 0X02,
  INFO = 0X03,
  DEBUG = 0X04,
  TRACE = 0X05,
};

class LOG {
 public:
  LOG() = default;
  explicit LOG(LogLevel level);
  ~LOG();

  template <typename T>
  LOG& operator<<(const T& message);

 private:
  LogLevel log_level_;
  std::ostringstream buffer_;
};

template <typename T>
LOG& LOG::operator<<(const T& message) {
  buffer_ << message;
  return *this;
}

}  // namespace nlink

#endif  // LINK_BASE_LOGGING_H_
