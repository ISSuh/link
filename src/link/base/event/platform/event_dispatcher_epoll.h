/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_

#include <vector>

#include "link/base/macro.h"
#include "link/base/platform/discriptor.h"
#include "link/base/event/event_dispatcher.h"

namespace link {
namespace base {

class EventDispatcherEpoll : public EventDispatcher {
 public:
  static EventDispatcherEpoll* CreateEventDispatcher(
    int32_t evnet_size, int32_t timeout);
  ~EventDispatcherEpoll();

  void Dispatch() override;

 private:
  EventDispatcherEpoll(Discriptor fd, int32_t event_size, int32_t timeout);

  bool AttachChannel(EventChannel* channel) override;
  void DetatchCahnnel(Discriptor fd) override;
  void DispatchEvent(const Event& event) override;

  Discriptor epoll_fd_;
  uint32_t event_size_;
  int32_t timeout_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherEpoll);
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
