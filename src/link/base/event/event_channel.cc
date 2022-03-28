/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event_channel.h"

namespace link {
namespace base {

EventChannel::EventChannel(EventObserver* observer)
  : observer_(observer) {}

EventChannel::~EventChannel() {
  CloseChannel();
}

void EventChannel::CloseChannel() {
  // controller_->DetatchCahnnel(observer_->discriptor());
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
