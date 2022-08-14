/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/base/component.h"

namespace nlink {
namespace component {

LinkComponent::LinkComponent(
  base::ComponentChannelController* channel_controller)
  : channel_controller_(channel_controller) {
}

LinkComponent::~LinkComponent() = default;

void LinkComponent::AttachChannelsToController(
  base::EventChannel* event_channel) {
  channel_controller_->AttachChannels(event_channel);
}

void LinkComponent::DetatchCahnnelFromController(
  base::EventChannel* event_channel) {
  channel_controller_->DetatchCahnnel(event_channel);
}

}  // namespace component
}  // namespace nlink
