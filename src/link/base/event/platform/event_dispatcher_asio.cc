/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/event_dispatcher_asio.h"

#include "link/third_party/asio/asio/io_context.hpp"

namespace nlink {
namespace base {

AsioDispatcherConext::AsioDispatcherConext()
  : context_(static_cast<void*>(new asio::io_context())) {
}

std::shared_ptr<void> AsioDispatcherConext::context() const {
  return context_;
}

EventDispatcherAsio* EventDispatcherAsio::CreateEventDispatcher() {
  return new EventDispatcherAsio();
}

EventDispatcherAsio::EventDispatcherAsio() {
}

EventDispatcherAsio::~EventDispatcherAsio() {
}

void EventDispatcherAsio::Dispatch() {

}

DispatcherConext* EventDispatcherAsio::GetDispatcherConext() {
  return dynamic_cast<DispatcherConext*>(&context_);
}

bool EventDispatcherAsio::AttachChannel(EventChannel* channel) {
  
  
}

void EventDispatcherAsio::DetatchCahnnel(EventChannel* channel) {

}

void EventDispatcherAsio::DispatchEvent(const Event& event) {

}

}  // namespace base
}  // namespace nlink
