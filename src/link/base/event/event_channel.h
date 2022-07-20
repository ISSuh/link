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

namespace nlink {
namespace base {

class DispatcherConext;

class EventChannel {
 public:
  class EventChannelDelegate {
    virtual void OnOpend(EventChannel* channel);
    virtual void OnClosed(EventChannel* channel);
  };

  virtual void OpenChannel(DispatcherConext* context) = 0;
  virtual void CloseChannel() = 0;
  virtual void HandleEvent(const Event& event) = 0;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_EVENT_CHANNEL_H_
