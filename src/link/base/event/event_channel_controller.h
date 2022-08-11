/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_

#include <map>
#include <functional>

#include "link/base/event/event.h"
#include "link/base/event/event_channel.h"

namespace nlink {
namespace base {

class EventChannel;

class EventChannelController : public EventChannel::EventChannelDelegate {
 public:
  using AttachChannelCallback = std::function<void(int32_t, EventChannel*)>;
  using DetachChannelCallback = std::function<void(int32_t, EventChannel*)>;
  using UpdateChannelCallback = std::function<void(int32_t, EventChannel*)>;

  static EventChannelController* Create(
    AttachChannelCallback attach_callback,
    DetachChannelCallback detach_callback,
    UpdateChannelCallback update_callback);

  virtual void AttachChannels(EventChannel* channel) = 0;
  virtual void DetatchCahnnel(EventChannel* channel) = 0;
  virtual void DispatchEvent(const Event& event) = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
