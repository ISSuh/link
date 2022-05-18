/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/event_dispatcher_asio.h"

#include "link/third_party/asio/asio/io_context.hpp"

namespace nlink {
namespace base {

class AsioDispatcherConext : public DispatcherConext {
 public:
  AsioDispatcherConext()
    : context_(1) {}

  void* context() const override {
    return (void*)(&context_);
  }

 private:
  asio::io_context context_;
};

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
  io_context->run();
}

DispatcherConext* EventDispatcherAsio::GetDispatcherConext() {
  return context_.get();
}

void EventDispatcherAsio::AttachChannel(EventChannel* channel) {
  channel->OpenChannel(context_.get());
}

void EventDispatcherAsio::DetatchCahnnel(EventChannel* channel) {
  channel->CloseChannel();
}

void EventDispatcherAsio::DispatchEvent(const Event& event) {
}

}  // namespace base
}  // namespace nlink
