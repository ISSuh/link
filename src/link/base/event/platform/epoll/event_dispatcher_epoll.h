/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_

#include <vector>
#include <memory>
#include <atomic>

#include "link/base/macro.h"
#include "link/base/platform/descriptor.h"
#include "link/base/event/event_channel.h"
#include "link/base/event/event_dispatcher.h"
#include "link/base/event/channel_controller.h"

namespace nlink {
namespace base {

class EventDispatcherEpoll : public EventDispatcher {
 public:
  static EventDispatcherEpoll* CreateEventDispatcher();

  EventDispatcherEpoll(
    int32_t epoll_descriptor,
    int32_t event_size,
    int32_t timeout);
  virtual ~EventDispatcherEpoll();

  // nlink::base::EventDispatcher
  void RegistEventChannelContoller(
    std::shared_ptr<ChannelController> channel_controller);
  void Dispatch() override;
  void DispatchOnce() override;
  void Stop() override;

 private:
  Event::Type HandleEventType(
    uint32_t event_flag, std::vector<Event::Type>* types);

  void OnAttachChannel(int32_t descriptor, EventChannel* channel);
  void OnDetachChannel(int32_t descriptor, EventChannel* channel);
  void OnUpdatedChannel(int32_t descriptor, EventChannel* channel);

  std::atomic_bool running_;

  uint32_t event_size_;
  int32_t timeout_;

  int32_t epoll_descriptor_;
  std::shared_ptr<EventChannelController> event_channel_controller_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherEpoll);
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
