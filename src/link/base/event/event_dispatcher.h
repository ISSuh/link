/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CONTROLLER_H_

#include <vector>
#include <memory>

#include "link/base/event/event_channel.h"
#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

class EventChannel;

class DispatcherConext {
 public:
  virtual ~DispatcherConext() = default;

  virtual void* context() const = 0;
};

class EventDispatcher {
 public:
  virtual ~EventDispatcher() = default;

  virtual void Dispatch() = 0;
  virtual void DispatchOnce() = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CONTROLLER_H_
