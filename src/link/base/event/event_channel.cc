/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event_channel.h"

namespace link {
namespace base {

EventChannel::EventChannel(
  EventChannelController* controller, EventObserver* observer)
  : controller_(controller), observer_(observer) {}

EventChannel::~EventChannel() {
  CloseChannel();
}

bool EventChannel::AttachChannelToController() {
  return controller_->AttachChannel(this);
}

void EventChannel::CloseChannel() {
  controller_->DetatchCahnnel(observer_->discriptor());
}

Discriptor EventChannel::ChannelDiscriptor() const {
  return observer_->discriptor();
}

EventObserver::Type EventChannel::ObserverType() const {
  return observer_->type();
}

void EventChannel::HandleEvent(const Event& event) {
  observer_->HandleEvent(event);
}

}  // namespace base
}  // namespace link
