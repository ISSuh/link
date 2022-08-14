/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_CHANNEL_CONTROLLER_H_
#define LINK_BASE_EVENT_CHANNEL_CONTROLLER_H_

#include <map>

#include "link/base/event/event.h"
#include "link/base/event/event_channel.h"
#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

class ChannelController
  : public ComponentChannelController,
    public EventChannelController {
 public:
  ChannelController();
  virtual ~ChannelController();

  void RegistEventChannelDelegateCallback(
    EventChannelController::AttachChannelCallback attach_callback,
    EventChannelController::DetachChannelCallback detach_callback,
    EventChannelController::UpdateChannelCallback update_callback);

  void CloseAllChannels();

 private:
  // ComponentChannelController
  void AttachChannels(EventChannel* channel) override;
  void DetatchCahnnel(EventChannel* channel) override;

  // EventChannelController
  void DispatchEvent(const Event& event) override;

  // EventChannel::EventChannelDelegate
  void ChannelOpend(int32_t descriptor, EventChannel* channel) override;
  void ChannelClosed(int32_t descriptor, EventChannel* channel) override;
  void UpdateChannel(int32_t descriptor, EventChannel* channel) override;

  EventChannelController::AttachChannelCallback attach_channel_callback_;
  EventChannelController::DetachChannelCallback detach_channel_callback_;
  EventChannelController::UpdateChannelCallback update_channel_callback_;

  std::map<int32_t, EventChannel*> channel_map_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_CHANNEL_CONTROLLER_H_
