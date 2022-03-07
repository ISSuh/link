/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event.h"

namespace link {
namespace base {

Event::Event(int32_t fd, Type type)
  : fd_(fd), type_(type) {
}

Event::Event(const Event& rhs)
  : fd_(rhs.fd_), type_(rhs.type_) {
}

Event::Event(Event&& rhs)
  : fd_(rhs.fd_), type_(rhs.type_) {
}

Event::~Event() {
}

Discriptor Event::discriptor() const {
  return fd_;
}

Event::Type Event::type() const {
  return type_;
}

Event& Event::operator=(const Event& rhs) {
  if (&rhs == this) {
    return *this;
  }

  fd_ = rhs.fd_;
  type_ = rhs.type_;
  return *this;
}

}  // namespace base
}  // namespace link
