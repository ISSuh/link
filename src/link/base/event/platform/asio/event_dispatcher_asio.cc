/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/asio/event_dispatcher_asio.h"

#include "link/base/event/platform/asio/dispatcher_context_asio.h"
#include "link/third_party/asio/asio/io_context.hpp"

namespace nlink {
namespace base {

EventDispatcherAsio* EventDispatcherAsio::CreateEventDispatcher() {
  return new EventDispatcherAsio();
}

EventDispatcherAsio::EventDispatcherAsio()
  : context_(new AsioDispatcherConext()) {
}

EventDispatcherAsio::~EventDispatcherAsio() {
}

void EventDispatcherAsio::Dispatch() {
  asio::io_context* io_context =
    static_cast<asio::io_context*>(context_->context());
  if (nullptr != io_context) {
    io_context->run();
  }
}

void EventDispatcherAsio::DispatchOnce() {
  asio::io_context* io_context =
    static_cast<asio::io_context*>(context_->context());
  if (nullptr != io_context) {
    io_context->poll();
  }
}

DispatcherConext* EventDispatcherAsio::GetDispatcherConext() {
  return context_.get();
}

void EventDispatcherAsio::AttachChannels(base::EventChannel* channel) {
  channel->OpenChannel(context_.get());
}

void EventDispatcherAsio::DetatchCahnnel(EventChannel* channel) {
  channel->CloseChannel();
}

void EventDispatcherAsio::DispatchEvent(const Event& event) {
}

}  // namespace base
}  // namespace nlink
