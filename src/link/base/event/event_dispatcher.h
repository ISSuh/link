/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CONTROLLER_H_

#include <memory>

#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

class DispatcherConext {
 public:
  virtual std::shared_ptr<void> context() const = 0;
};

class EventDispatcher : public EventChannelController {
 public:
  virtual void Dispatch() = 0;
  virtual DispatcherConext* GetDispatcherConext() = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CONTROLLER_H_
