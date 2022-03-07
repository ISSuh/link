/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
#define LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_

#include "link/base/event/platform/event_dispatcher_epoll.h"

namespace link {
namespace base {

class EventDispatcherFactory {
 public:
  static EventDispatcher* CreateEventDispatcher(
    int32_t evnet_size, int32_t timeout) {
    return EventDispatcherEpoll::CreateEventDispatcher(evnet_size, timeout);
  }
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_DISPATCHER_FACTORY_H_
