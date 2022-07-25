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
  using RegistChannelCallback = std::function<void(int32_t)>;

  explicit EventChannelController(RegistChannelCallback regist_callback);
  ~EventChannelController();

  void AttachChannels(EventChannel* channel);
  void DetatchCahnnel(EventChannel* channel);
  void DispatchEvent(const Event& event);

  // EventChannel::EventChannelDelegate
  void OnOpend(EventChannel* channel) override;
  void OnClosed(EventChannel* channel) override;

 private:
  RegistChannelCallback regist_channel_callback_;
  std::map<int32_t, EventChannel*> channel_map_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
