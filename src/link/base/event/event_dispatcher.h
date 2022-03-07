/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CONTROLLER_H_

#include "link/base/event/event_channel_controller.h"

namespace link {
namespace base {

class EventChannelController;

class EventDispatcher
  : public EventChannelController {
 public:
  virtual void Dispatch() = 0;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_CONTROLLER_H_
