/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_EVENT_CONTROLLER_H_
#define LINK_BASE_EVNET_EVENT_CONTROLLER_H_

#include "link/base/evnet/event_channel_controller.h"

namespace link {
namespace base {

class EventDispatcher
  : public EventChannelController {
 public:
  virtual void Dispatch() = 0;
};

class EventLoop {
 public:
  void RunOnce() {
    event_dispatcher_->Dispatch();
  }

  void Run() {
    while (true) {
      event_dispatcher_->Dispatch();
    }
  }

 private:
  EventDispatcher* event_dispatcher_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_EVENT_CONTROLLER_H_
