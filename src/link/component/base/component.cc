/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/base/component.h"

namespace nlink {
namespace component {

void LinkComponent::RegistEventChannelObserver(
  base::EventChannelObserver* channel_observer) {
  channel_observer_ = channel_observer;
}

void LinkComponent::AttachChannelsToObserver(
  base::EventChannel* event_channel) {
  channel_observer_->AttachChannels(event_channel);
}

void LinkComponent::DetatchCahnnelFromObserver(
  base::EventChannel* event_channel) {
  channel_observer_->DetatchCahnnel(event_channel);
}


}  // namespace component
}  // namespace nlink
