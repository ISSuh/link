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
  : event_dispatcher_(nullptr) {
}

LinkHandle::~LinkHandle() {
}

void LinkHandle::Initialize() {
  int32_t evnet_size = 5;
  int32_t timeout = -1;

  base::EventDispatcher* dispatcher =
    base::EventDispatcherFactory::CreateEventDispatcher(evnet_size, timeout);
  if (!dispatcher) {
    return;
  }

  event_dispatcher_.reset(dispatcher);
}

void LinkHandle::RunOnce() {
  event_dispatcher_->Dispatch();
}

void LinkHandle::Shutdown() {
}

void LinkHandle::RegistComponent(component::LinkComponent* component) {
  base::EventChannel* channel = component->GetEventChannel();
  event_dispatcher_->AttachChannel(channel);
}

}  // namespace handle
}  // namespace nlink
