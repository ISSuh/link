/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
#define LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_

#include <memory>

#include "link/base/event/event_channel_controller.h"
#include "link/base/event/platform/epoll/event_dispatcher_epoll.h"

namespace nlink {
namespace base {

class EventDispatcherFactory {
 public:
  static EventDispatcher* CreateEventDispatcher() {
    return EventDispatcherEpoll::CreateEventDispatcher();
  }
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
