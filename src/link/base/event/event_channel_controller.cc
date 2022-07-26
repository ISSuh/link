/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

class EventChannelControllerImpl : public EventChannelController {
 public:
  explicit EventChannelControllerImpl(
    EventChannelController::AttachChannelCallback attach_callback,
    EventChannelController::DetachChannelCallback detach_callback,
    EventChannelController::UpdateChannelCallback update_callback);
  ~EventChannelControllerImpl();

  void AttachChannels(EventChannel* channel);
  void DetatchCahnnel(EventChannel* channel);
  void DispatchEvent(const Event& event);

 private:
  // EventChannel::EventChannelDelegate
  void ChannelOpend(int32_t descriptor, EventChannel* channel) override;
  void ChannelClosed(int32_t descriptor, EventChannel* channel) override;
  void UpdateChannel(int32_t descriptor, EventChannel* channel) override;

  EventChannelController::AttachChannelCallback attach_channel_callback_;
  EventChannelController::DetachChannelCallback detach_channel_callback_;
  EventChannelController::UpdateChannelCallback update_channel_callback_;

  std::map<int32_t, EventChannel*> channel_map_;
};

EventChannelControllerImpl::EventChannelControllerImpl(
  EventChannelController::AttachChannelCallback attach_callback,
  EventChannelController::DetachChannelCallback detach_callback,
  EventChannelController::UpdateChannelCallback update_callback)
  : attach_channel_callback_(attach_callback),
    detach_channel_callback_(detach_callback),
    update_channel_callback_(update_callback) {
}

EventChannelControllerImpl::~EventChannelControllerImpl() {
}

void EventChannelControllerImpl::AttachChannels(EventChannel* channel) {
  channel->OpenChannel(this);
}

void EventChannelControllerImpl::DetatchCahnnel(EventChannel* channel) {
  channel->CloseChannel();
}

void EventChannelControllerImpl::DispatchEvent(const Event& event) {
  int32_t descriptor = event.Descriptor();
  EventChannel* channel = channel_map_.at(descriptor);
  channel->HandleEvent(event);
}

void EventChannelControllerImpl::ChannelOpend(
  int32_t descriptor, EventChannel* channel) {
  channel_map_.insert({descriptor, channel});
  attach_channel_callback_(descriptor);
}

void EventChannelControllerImpl::ChannelClosed(
  int32_t descriptor, EventChannel* channel) {
  detach_channel_callback_(descriptor);
  channel_map_.erase(descriptor);
}

void EventChannelControllerImpl::UpdateChannel(
  int32_t descriptor, EventChannel* channel) {
  update_channel_callback_(descriptor);
}

EventChannelController* EventChannelController::Create(
  AttachChannelCallback attach_callback,
  DetachChannelCallback detach_callback,
  UpdateChannelCallback update_callback) {
  return new EventChannelControllerImpl(
    attach_callback, detach_callback, update_callback);
}

}  // namespace base
}  // namespace nlink
