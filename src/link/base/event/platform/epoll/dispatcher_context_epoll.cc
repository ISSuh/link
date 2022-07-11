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

bool EpollDispatcherConext::Regist(void* target) {
  int32_t file_discriptor = *reinterpret_cast<int32_t*>(target);
  epoll_event event;
  event.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLET;
  event.data.fd = file_discriptor;

  if (epoll_ctl(discriptor_, EPOLL_CTL_ADD, file_discriptor, &event) < 0) {
    return false;
  }
  return true;
}

bool EpollDispatcherConext::Unregist(void* target) {
  int32_t file_discriptor = *reinterpret_cast<int32_t*>(target);
  if (epoll_ctl(discriptor_, EPOLL_CTL_DEL, file_discriptor, nullptr) < 0) {
    return false;
  }
  return true;
}

}  // namespace base
}  // namespace nlink
