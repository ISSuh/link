/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_H_

#include <unordered_map>

#include "link/base/platform/discriptor.h"
#include "link/base/event/event.h"
#include "link/base/event/event_observer.h"
#include "link/base/event/event_channel_controller.h"

namespace link {
namespace base {

class EventChannelController;

class EventChannel {
 public:
  EventChannel(EventChannelController* controller, EventObserver* observer);
  virtual ~EventChannel();

  bool AttachChannelToController();
  void CloseChannel();

  Discriptor ChannelDiscriptor() const;
  EventObserver::Type ObserverType() const;
  void HandleEvent(const Event& event);

 private:
  EventChannelController* controller_;
  EventObserver* observer_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_H_
