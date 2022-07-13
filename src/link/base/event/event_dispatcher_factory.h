/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
#define LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_

#include "link/base/event/platform/asio/event_dispatcher_asio.h"
#include "link/base/event/platform/epoll/event_dispatcher_epoll.h"

namespace nlink {
namespace base {

class EventDispatcherFactory {
 public:
  static EventDispatcher* CreateEventDispatcher() {
    // return EventDispatcherAsio::CreateEventDispatcher();
    return EventDispatcherEpoll::CreateEventDispatcher();
  }
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
