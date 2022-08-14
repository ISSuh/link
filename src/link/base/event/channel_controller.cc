/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/channel_controller.h"

namespace nlink {
namespace base {

ChannelController::ChannelController() = default;

ChannelController::~ChannelController() = default;

void ChannelController::RegistEventChannelDelegateCallback(
  EventChannelController::AttachChannelCallback attach_callback,
  EventChannelController::DetachChannelCallback detach_callback,
  EventChannelController::UpdateChannelCallback update_callback) {
  attach_channel_callback_ = attach_callback;
  detach_channel_callback_ = detach_callback;
  update_channel_callback_ = update_callback;
}

void ChannelController::CloseAllChannels() {
  for (const auto& item : channel_map_) {
    EventChannel* channel = item.second;
    DetatchCahnnel(channel);
  }
}

void ChannelController::AttachChannels(EventChannel* channel) {
  channel->OpenChannel(this);
}

void ChannelController::DetatchCahnnel(EventChannel* channel) {
  channel->CloseChannel();
}

void ChannelController::DispatchEvent(const Event& event) {
  int32_t descriptor = event.Descriptor();
  EventChannel* channel = channel_map_.at(descriptor);
  channel->HandleEvent(event);
}

void ChannelController::ChannelOpend(
  int32_t descriptor, EventChannel* channel) {
  channel_map_.insert({descriptor, channel});
  attach_channel_callback_(descriptor, channel);
}

void ChannelController::ChannelClosed(
  int32_t descriptor, EventChannel* channel) {
  detach_channel_callback_(descriptor, channel);
  channel_map_.erase(descriptor);
}

void ChannelController::UpdateChannel(
  int32_t descriptor, EventChannel* channel) {
  update_channel_callback_(descriptor, channel);
}

}  // namespace base
}  // namespace nlink
