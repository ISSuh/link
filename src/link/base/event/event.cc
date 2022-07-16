/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event.h"

namespace nlink {
namespace base {

Event::Event(int32_t fd, const std::vector<Event::Type>& type)
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

int32_t Event::Discriptor() const {
  return fd_;
}

const std::vector<Event::Type>& Event::Types() const {
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
}  // namespace nlink
