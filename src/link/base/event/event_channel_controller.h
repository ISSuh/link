/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_

#include "link/base/event/event_channel.h"

namespace nlink {
namespace base {

class EventChannel;

class EventChannelController {
 public:
  friend EventChannel;

  virtual bool AttachChannel(EventChannel* channel) = 0;
  virtual void DetatchCahnnel(EventChannel* channel) = 0;
  virtual void DispatchEvent(const Event& event) = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_CONTROLLER_H_
