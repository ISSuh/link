/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_
#define LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_

#include <vector>
#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_dispatcher.h"

namespace nlink {
namespace base {

class EventDispatcherAsio : public EventDispatcher {
 public:
  static EventDispatcherAsio* CreateEventDispatcher();

  virtual ~EventDispatcherAsio();

  // nlink::base::EventDispatcher
  void Dispatch() override;
  void DispatchOnce() override;

 private:
  EventDispatcherAsio();

  std::unique_ptr<DispatcherConext> context_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherAsio);
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EVENT_DISPATCHER_ASIO_H_
