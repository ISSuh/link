/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/epoll/dispatcher_context_epoll.h"

#include <sys/unistd.h>
#include <sys/epoll.h>

namespace nlink {
namespace base {

EpollDispatcherConext::EpollDispatcherConext(int32_t discriptor)
  : discriptor_(discriptor) {
}

EpollDispatcherConext::~EpollDispatcherConext() {
}

void* EpollDispatcherConext::context() const {
  return (void*)(&discriptor_);
}

bool EpollDispatcherConext::Regist(int32_t handle, EventChannel* channel) {
  epoll_event event;
  event.events = EPOLLOUT | EPOLLERR | EPOLLRDHUP;
  event.data.fd = handle;

  if (epoll_ctl(discriptor_, EPOLL_CTL_ADD, handle, &event) < 0) {
    return false;
  }

  channels.emplace(handle, channel);
  return true;
}

bool EpollDispatcherConext::Unregist(int32_t handle) {
  if (epoll_ctl(discriptor_, EPOLL_CTL_DEL, handle, nullptr) < 0) {
    return false;
  }
  channels.erase(handle);
  return true;
}

void EpollDispatcherConext::Dispatch(const Event& event) {
  int32_t channel_discriptor = event.Discriptor();
  channels[channel_discriptor]->HandleEvent(event);
}

}  // namespace base
}  // namespace nlink
