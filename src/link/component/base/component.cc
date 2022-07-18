/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/base/component.h"

namespace nlink {
namespace component {

LinkComponent::LinkComponent(base::EventChannelObserver* channel_subject)
  : channel_subject_(channel_subject) {
}

LinkComponent::~LinkComponent() {
}

void LinkComponent::AttachChannelsToObserver(
  base::EventChannel* event_channel) {
  channel_subject_->AttachChannels(event_channel);
}

void LinkComponent::DetatchCahnnelFromObserver(
  base::EventChannel* event_channel) {
  channel_subject_->DetatchCahnnel(event_channel);
}

}  // namespace component
}  // namespace nlink
