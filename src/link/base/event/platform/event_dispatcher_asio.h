/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_dispatcher.h"

namespace nlink {
namespace base {

class AsioDispatcherConext : public DispatcherConext {
 public:
  AsioDispatcherConext();
  std::shared_ptr<void> context() const override;

 private:
  std::shared_ptr<void> context_;
};

class EventDispatcherAsio : public EventDispatcher {
 public:
  static EventDispatcherAsio* CreateEventDispatcher();

  virtual ~EventDispatcherAsio();

  void Dispatch() override;
  DispatcherConext* GetDispatcherConext() override;

  bool AttachChannel(EventChannel* channel) override;
  void DetatchCahnnel(EventChannel* channel) override;
  void DispatchEvent(const Event& event) override;

 private:
  EventDispatcherAsio();

  AsioDispatcherConext context_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherAsio);
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_
