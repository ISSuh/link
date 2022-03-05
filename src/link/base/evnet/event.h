/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_EVENT_H_
#define LINK_BASE_EVNET_EVENT_H_

#include "link/base/platform/discriptor.h"

namespace link {
namespace base {

class Event {
 public:
  enum class Type : uint8_t {
    NONE,
    READ,
    WRITE,
    TIMEOUT,
    CLOSE,
    ERROR
  };

  Event(int32_t fd, Type type) {}

  Discriptor fd() const { return fd_; }

  Event::Type type() const {
    return type_;
  }

 private:
  Discriptor fd_;
  Type type_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_EVENT_H_
