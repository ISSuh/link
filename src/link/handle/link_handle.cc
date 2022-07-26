/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/handle/link_handle.h"

#include "link/base/event/event_channel.h"
#include "link/base/event/event_dispatcher_factory.h"
#include "link/base/logging.h"

namespace nlink {
namespace handle {

LinkHandle::LinkHandle()
  : event_dispatcher_(nullptr),
    channel_controller_(nullptr) {
}

LinkHandle::~LinkHandle() {
}

void LinkHandle::Initialize() {
  base::EventDispatcher* dispatcher =
    base::EventDispatcherFactory::CreateEventDispatcher();
  if (!dispatcher) {
    return;
  }

  event_dispatcher_.reset(dispatcher);
}

void LinkHandle::Run() {
  if (!event_dispatcher_) {
    return;
  }
  event_dispatcher_->Dispatch();
}

void LinkHandle::RunOnce() {
  if (!event_dispatcher_) {
    return;
  }
  event_dispatcher_->DispatchOnce();
}

void LinkHandle::Shutdown() {
}

void LinkHandle::RegistComponent(component::LinkComponent* component) {
  if (!event_dispatcher_) {
    return;
  }
}

void LinkHandle::RegistEventChannls(base::EventChannel* channel) {
  // event_dispatcher_->AttachChannels(channel);
}

}  // namespace handle
}  // namespace nlink
