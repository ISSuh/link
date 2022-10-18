/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/time.h"

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace nlink {
namespace base {

constexpr const int64_t kNanosecondsPerMicrosecond = 1000;
constexpr const int64_t kMicrosecondsPerMillisecond = 1000;

class Time;

TimeTick TimeTick::FromMilliseconds(int64_t ms) {
  return TimeTick::From(ms, kMicrosecondsPerMillisecond);
}

TimeTick TimeTick::Now() {
  return TimeTick::From(Time::InMicrosecond(), 1);
}

TimeTick TimeTick::From(int64_t value, int64_t offset) {
  return TimeTick(value * offset);
}

TimeTick::TimeTick()
  : tick_(0) {
}

TimeTick::TimeTick(const TimeTick& tick)
  : tick_(tick.tick_) {
}

TimeTick::TimeTick(int64_t us)
  : tick_(us < 0 ? 0 : us) {
}

TimeTick::~TimeTick() = default;

int64_t TimeTick::Tick() const {
  return tick_;
}

TimeTick& TimeTick::operator=(TimeTick other) {
  tick_ = other.tick_;
  return *this;
}

TimeTick TimeTick::operator+(TimeTick other) const {
  return TimeTick(tick_ + other.tick_);
}

TimeTick TimeTick::operator-(TimeTick other) const {
  return TimeTick(tick_ - other.tick_);
}

TimeTick& TimeTick::operator+=(TimeTick other) {
  return *this = (*this + other);
}

TimeTick& TimeTick::operator-=(TimeTick other) {
  return *this = (*this - other);
}

TimeTick TimeTick::operator-() const {
  return TimeTick(-tick_);
}

bool TimeTick::operator==(TimeTick other) const {
  return tick_ == other.tick_;
}

bool TimeTick::operator!=(TimeTick other) const {
  return tick_ != other.tick_;
}

bool TimeTick::operator<(TimeTick other) const {
  return tick_ < other.tick_;
}

bool TimeTick::operator<=(TimeTick other) const {
  return tick_ <= other.tick_;
}

bool TimeTick::operator>(TimeTick other) const {
  return tick_ > other.tick_;
}

bool TimeTick::operator>=(TimeTick other) const {
  return tick_ >= other.tick_;
}

int64_t Time::InNanosecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
      now.time_since_epoch()).count();
}

int64_t Time::InMicrosecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
      now.time_since_epoch()).count();
}

int64_t Time::InMillisecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()).count();
}

int64_t Time::InSecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(
      now.time_since_epoch()).count();
}

std::string Time::CurrentTimeToDateStr() {
  auto now = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm current_local_time = *std::localtime(&current_time);

  const auto ms =
    std::chrono::time_point_cast<std::chrono::microseconds>(now)
      .time_since_epoch().count() % 1000000;

  std::ostringstream date_str_stream;

  date_str_stream << std::put_time(&current_local_time, "%Y-%m-%d %H:%M:%S");
  date_str_stream << '.' << std::setfill('0') << std::setw(6) << ms;

  return date_str_stream.str();
}

Time Time::Now() {
  return Time(Time::InMicrosecond());
}

Time::Time()
  : time_tick_(0) {
}

Time::Time(int64_t us)
  : time_tick_(us) {
}

Time::Time(TimeTick tick)
  : time_tick_(tick) {
}

Time::~Time() = default;

Time Time::operator+(Time other) const {
  return Time(time_tick_ + other.time_tick_);
}

Time Time::operator-(Time other) const {
  return Time(time_tick_ - other.time_tick_);
}

bool Time::operator==(Time other) const {
  return time_tick_ == other.time_tick_;
}

bool Time::operator!=(Time other) const {
  return time_tick_ != other.time_tick_;
}

bool Time::operator<(Time other) const {
  return time_tick_ < other.time_tick_;
}

bool Time::operator<=(Time other) const {
  return time_tick_ <= other.time_tick_;
}

bool Time::operator>(Time other) const {
  return time_tick_ > other.time_tick_;
}

bool Time::operator>=(Time other) const {
  return time_tick_ >= other.time_tick_;
}

}  // namespace base
}  // namespace nlink
