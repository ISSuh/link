/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_CHANNEL_H_
#define LINK_BASE_EVENT_EVENT_CHANNEL_H_

#include "link/base/event/event.h"

namespace nlink {
namespace base {

class DispatcherConext;

class EventChannel {
 public:
  class EventChannelDelegate {
   public:
    virtual void ChannelOpend(int32_t descriptor, EventChannel* channel) = 0;
    virtual void ChannelClosed(int32_t descriptor, EventChannel* channel) = 0;
    virtual void UpdateChannel(int32_t descriptor, EventChannel* channel) = 0;
  };

  virtual void OpenChannel(EventChannelDelegate* delegate) = 0;
  virtual void CloseChannel() = 0;
  virtual void HandleEvent(const Event& event) = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_H_
