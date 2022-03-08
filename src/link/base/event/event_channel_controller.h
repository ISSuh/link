/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_

#include <unordered_map>

#include "link/base/event/event_channel.h"

namespace link {
namespace base {

class EventChannel;

class EventChannelController {
 public:
  friend EventChannel;

  virtual bool AttachChannel(EventChannel* channel) = 0;
  virtual void DetatchCahnnel(Discriptor fd) = 0;
  virtual void DispatchEvent(const Event& event) = 0;

 protected:
  std::unordered_map<Discriptor, EventChannel*> channel_map_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
