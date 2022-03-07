/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_H_
#define LINK_BASE_EVENT_EVENT_H_

#include "link/base/platform/discriptor.h"

namespace link {
namespace base {

class Event {
 public:
  enum class Type : int8_t {
    ERROR = -1,
    NONE = 0,
    ACCEPT,
    CONNECT,
    READ,
    WRITE,
    TIMEOUT,
    CLOSE,
    MAX = ERROR
  };

  Event(int32_t fd, Type type);
  Event(const Event& rhs);
  Event(Event&& rhs);
  ~Event();

  Discriptor discriptor() const;
  Event::Type type() const;

  Event& operator=(const Event& rhs);

 private:
  Discriptor fd_;
  Type type_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_H_
