/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_EVENT_CHANNEL_CLIENT_H_
#define LINK_BASE_EVNET_EVENT_CHANNEL_CLIENT_H_

namespace link {
namespace base {

class EventChannelClient {
 public:
  EventChannelClient() = default;
  virtual ~EventChannelClient() = default;

  virtual void CloseChannel() = 0;

  virtual int32_t FileDiscriptor() = 0;
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_EVENT_CHANNEL_CLIENT_H_
