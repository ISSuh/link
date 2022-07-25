/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event_channel_controller.h"

namespace nlink {
namespace base {

EventChannelController::EventChannelController(
  RegistChannelCallback regist_callback) {

}

EventChannelController::~EventChannelController() {
}

void EventChannelController::AttachChannels(EventChannel* channel) {
}

void EventChannelController::DetatchCahnnel(EventChannel* channel) {

}

void EventChannelController::DispatchEvent(const Event& event) {

}

void EventChannelController::OnOpend(EventChannel* channel) {

}

void EventChannelController::OnClosed(EventChannel* channel) {

}

}  // namespace base
}  // namespace nlink
