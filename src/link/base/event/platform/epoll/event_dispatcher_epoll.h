/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_

#include <vector>
#include <memory>
#include <unordered_map>

#include "link/base/macro.h"
#include "link/base/platform/discriptor.h"
#include "link/base/event/event_channel.h"
#include "link/base/event/event_dispatcher.h"
#include "link/base/event/event_channel_controller.h"
#include "link/base/event/platform/epoll/dispatcher_context_epoll.h"

namespace nlink {
namespace base {

class EventDispatcherEpoll : public EventDispatcher {
 public:
  static EventDispatcherEpoll* CreateEventDispatcher();

  EventDispatcherEpoll(
    std::unique_ptr<EpollDispatcherConext> context,
    int32_t event_size,
    int32_t timeout);
  virtual ~EventDispatcherEpoll();

  // nlink::base::EventDispatcher
  void Dispatch() override;
  void DispatchOnce() override;
  DispatcherConext* GetDispatcherConext() override;
  void DispatchEvent(const Event& event) override;

  // nlink::base::EventChannelObserver
  void AttachChannels(base::EventChannel* channel) override;
  void DetatchCahnnel(EventChannel* channel) override;

 private:
  Event::Type HandlingServerEvent();
  Event::Type HandlingClientEvent(
    uint32_t event_flag, std::vector<Event::Type>* types);

  std::unordered_map<Discriptor, EventChannel*> channel_map_;

  uint32_t event_size_;
  int32_t timeout_;

  std::unique_ptr<EpollDispatcherConext> context_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherEpoll);
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
