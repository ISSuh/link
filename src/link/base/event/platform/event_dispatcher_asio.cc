/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/event_dispatcher_asio.h"

#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/executor_work_guard.hpp"

namespace nlink {
namespace base {

class AsioDispatcherConext : public DispatcherConext {
 public:
  AsioDispatcherConext()
    : context_(1),
      work_guard_(context_.get_executor()) {}

  virtual ~AsioDispatcherConext() = default;

  void* context() const override {
    return (void*)(&context_);
  }

 private:
  asio::io_context context_;
  asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
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
