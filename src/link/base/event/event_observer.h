/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_OBSERVER_H_
#define LINK_BASE_EVENT_EVENT_OBSERVER_H_

#include "link/base/platform/discriptor.h"
#include "link/base/event/event.h"

namespace nlink {
namespace base {

class EventObserver {
 public:
  enum class Type : uint8_t{
    SERVER,
    CLIENT
  };

  virtual Discriptor discriptor() = 0;
  virtual Type type() = 0;
  virtual void HandleEvent(const Event& event) = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_OBSERVER_H_
