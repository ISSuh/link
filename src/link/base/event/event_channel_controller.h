/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_

#include <functional>

#include "link/base/event/event.h"
#include "link/base/event/event_channel.h"

namespace nlink {
namespace base {

class EventChannel;

class EventChannelController : public EventChannel::EventChannelDelegate {
 public:
  using RegistChannel = std::function<void(int32_t)>;

  EventChannelController() {}
  ~EventChannelController() {}

  void AttachChannels(EventChannel* channel) {}
  void DetatchCahnnel(EventChannel* channel) {}
  void DispatchEvent(const Event& event) {}

  // EventChannel::EventChannelDelegate
  void OnOpend(EventChannel* channel) override {}
  void OnClosed(EventChannel* channel) override {}

 private:
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
