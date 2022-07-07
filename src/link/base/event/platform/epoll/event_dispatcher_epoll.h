/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_

#include <vector>
#include <unordered_map>

#include "link/base/macro.h"
#include "link/base/platform/discriptor.h"
#include "link/base/event/event_channel.h"
#include "link/base/event/event_dispatcher.h"
#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

class EventDispatcherEpoll
  : public EventDispatcher,
    public EventChannelController {
 public:
  static EventDispatcherEpoll* CreateEventDispatcher(
    int32_t evnet_size, int32_t timeout);
  ~EventDispatcherEpoll();

  void Dispatch() override;

 private:
  EventDispatcherEpoll(Discriptor fd, int32_t event_size, int32_t timeout);

  Event::Type HandlingServerEvent();
  Event::Type HandlingClientEvent(uint32_t event_flag);

  bool AttachChannel(EventChannel* channel) override;
  void DetatchCahnnel(Discriptor fd) override;
  void DispatchEvent(const Event& event) override;

  std::unordered_map<Discriptor, EventChannel*> channel_map_;

  Discriptor epoll_fd_;
  uint32_t event_size_;
  int32_t timeout_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherEpoll);
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
