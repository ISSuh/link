/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_EVENT_CHANNEL_H_
#define LINK_BASE_EVNET_EVENT_CHANNEL_H_

#include <string>
#include <unordered_map>

#include "link/base/evnet/event.h"
#include "link/base/evnet/event_channel_controller.h"
#include "link/base/evnet/event_observer.h"

namespace link {
namespace base {

class EventChannel {
 public:
  EventChannel(
    EventChannelController* controller, EventObserver* observer)
      : controller_(controller), observer_(observer) {}

  virtual ~EventChannel() {
    CloseChannel();
  }

  bool AttachChannelToController() {
    return controller_->AttachChannel(this);
  }

  void CloseChannel() {
    controller_->DetatchCahnnel(observer_->discriptor());
  }

  Discriptor ChannelDiscriptor() const {
    return observer_->discriptor();
  }

  void HandleEvent(const Event& event) {
    switch (event.type()) {
      case Event::Type::READ:
        Read();
        break;
      case Event::Type::WRITE:
        Write();
        break;
      default:
        break;
    }
  }

 private:
  void Read() {
    observer_->Read();
  }

  void Write() {
    observer_->Write();
  }

  EventChannelController* controller_;
  EventObserver* observer_;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_EVENT_CHANNEL_H_
