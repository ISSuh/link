/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TIME_H_
#define LINK_BASE_TIME_H_

#include <stddef.h>
#include <string>

namespace nlink {
namespace base {

class TimeTick {
 public:
  static constexpr const int64_t kNanosecondsPerMicrosecond = 1000;
  static constexpr const int64_t kMicrosecondsPerMillisecond = 1000;

  static TimeTick FromMilliseconds(int64_t ms);
  static TimeTick Now();

  TimeTick();
  TimeTick(const TimeTick& tick);
  explicit TimeTick(int64_t us);
  ~TimeTick();

  int64_t Tick() const;

  TimeTick operator+(TimeTick other) const;
  TimeTick operator-(TimeTick other) const;
  TimeTick& operator+=(TimeTick other);
  TimeTick& operator-=(TimeTick other);
  TimeTick operator-() const;
  bool operator==(TimeTick other) const;
  bool operator!=(TimeTick other) const;
  bool operator<(TimeTick other) const;
  bool operator<=(TimeTick other) const;
  bool operator>(TimeTick other) const;
  bool operator>=(TimeTick other) const;

 private:
  static TimeTick From(int64_t value, int64_t offset);

  // microseconds
  int64_t tick_;
};

class Time {
 public:
  static int64_t InNanosecond();
  static int64_t InMicrosecond();
  static int64_t InMillisecond();
  static int64_t InSecond();

  static std::string CurrentTimeToDateStr();

  static Time Now();

  Time();
  explicit Time(int64_t us);
  explicit Time(TimeTick tick);
  ~Time();

  Time operator+(Time other) const;
  Time operator-(Time other) const;
  bool operator==(Time other) const;
  bool operator!=(Time other) const;
  bool operator<(Time other) const;
  bool operator<=(Time other) const;
  bool operator>(Time other) const;
  bool operator>=(Time other) const;

 private:
  TimeTick time_tick_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TIME_H_
