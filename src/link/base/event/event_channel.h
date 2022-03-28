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

namespace link {
namespace base {

class EventChannelController;

class EventChannel {
 public:
  EventChannel(EventObserver* observer);
  virtual ~EventChannel();

  void CloseChannel();

  Discriptor ChannelDiscriptor() const;
  EventObserver::Type ObserverType() const;
  void HandleEvent(const Event& event);

 private:
  EventObserver* observer_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_H_
