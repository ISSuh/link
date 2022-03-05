/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_EVENT_OBSERVER_H_
#define LINK_BASE_EVNET_EVENT_OBSERVER_H_

#include "link/base/platform/discriptor.h"

namespace link {
namespace base {

class EventObserver {
 public:
  virtual Discriptor discriptor() = 0;
  virtual void Read() = 0;
  virtual void Write() = 0;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_EVENT_OBSERVER_H_
