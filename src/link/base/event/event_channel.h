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

namespace nlink {
namespace base {

class EventChannel {
 public:
  virtual void OpenChannel(DispatcherConext* context) = 0;
  virtual void CloseChannel() = 0;
  virtual void HandleEvent(const Event& event) = 0;
};

// class EventChannel {
//  public:
//   explicit EventChannel(EventObserver* observer);
//   virtual ~EventChannel();

//   void CloseChannel();

//   Discriptor ChannelDiscriptor() const;
//   EventObserver::Type ObserverType() const;
//   void HandleEvent(const Event& event);

//  private:
//   EventObserver* observer_;
// };

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_H_
