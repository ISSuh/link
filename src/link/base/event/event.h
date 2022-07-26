/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_H_
#define LINK_BASE_EVENT_EVENT_H_

#include <cstdint>
#include <vector>

namespace nlink {
namespace base {

class Event {
 public:
  enum class Type : uint8_t {
    NONE = 0,
    ACCEPT = (1 << 0),
    CONNECT = (1 << 1),
    READ = (1 << 2),
    WRITE = (1 << 3),
    TIMEOUT = (1 << 4),
    CLOSE = (1 << 5),
    ERROR = (1 << 6),
    MAX = ERROR
  };

  Event(int32_t fd, const std::vector<Event::Type>& type);
  Event(const Event& rhs);
  Event(Event&& rhs);
  ~Event();

  int32_t Descriptor() const;
  const std::vector<Event::Type>&  Types() const;

  Event& operator=(const Event& rhs);

 private:
  int32_t fd_;
  std::vector<Type> type_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_H_
