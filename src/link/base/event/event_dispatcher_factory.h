/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
#define LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_

// #include "link/base/event/platform/event_dispatcher_epoll.h"
#include "link/base/event/platform/event_dispatcher_asio.h"

namespace nlink {
namespace base {

class EventDispatcherFactory {
 public:
  static EventDispatcher* CreateEventDispatcher() {
    return EventDispatcherAsio::CreateEventDispatcher();
  }
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
