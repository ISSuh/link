/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/epoll/event_dispatcher_epoll.h"

#include <sys/epoll.h>
#include <sys/unistd.h>

#include <vector>
#include <array>
#include <utility>

#include "link/base/logging.h"
#include "link/base/event/event_util.h"

namespace nlink {
namespace base {

using EpollEvent = epoll_event;
constexpr const int32_t kDefaultEvnetSize = 1024;
const int32_t kDefaultTimeOut = 100;

EventDispatcherEpoll* EventDispatcherEpoll::CreateEventDispatcher(
  std::shared_ptr<EventChannelController> channel_controller) {
  Descriptor epoll_fd = epoll_create(kDefaultEvnetSize);
  if (epoll_fd < 0) {
    close(epoll_fd);
    return nullptr;
  }

  return new EventDispatcherEpoll(
    epoll_fd, kDefaultEvnetSize, kDefaultTimeOut, channel_controller);
}

EventDispatcherEpoll::EventDispatcherEpoll(
  int32_t epoll_descriptor,
  int32_t event_size,
  int32_t timeout,
  std::shared_ptr<EventChannelController> channel_controller)
  : epoll_descriptor_(epoll_descriptor),
    event_size_(event_size),
    timeout_(timeout),
    channel_controller_(channel_controller) {
}

EventDispatcherEpoll::~EventDispatcherEpoll() {
}

void EventDispatcherEpoll::Dispatch() {
  while (1) {
    DispatchOnce();
  }
}

void EventDispatcherEpoll::DispatchOnce() {
  std::array<EpollEvent, kDefaultEvnetSize> epoll_events;
  int32_t event_count =
    epoll_wait(epoll_descriptor_, &epoll_events[0], event_size_, timeout_);
  if (event_count < 0) {
    LOG(INFO) << __func__ << " - Error";
    return;
  } else if (event_count == 0) {
    return;
  }

  LOG(INFO) << __func__ << " - event_count : " << event_count;


  for (int i = 0 ; i < event_count ; ++i) {
    int32_t fd = epoll_events[i].data.fd;
    uint32_t event_flag = epoll_events[i].events;

    std::vector<Event::Type> types;

    HandleEventType(event_flag, &types);

    Event event(fd, types);
    channel_controller_->DispatchEvent(event);
  }
}

Event::Type EventDispatcherEpoll::HandleEventType(
  uint32_t event_flag, std::vector<Event::Type>* types) {
  if (event_flag & EPOLLIN) {
    types->emplace_back(Event::Type::READ);
  }

  if (event_flag & EPOLLOUT) {
    types->emplace_back(Event::Type::WRITE);
  }

  if (event_flag & EPOLLERR) {
    types->emplace_back(Event::Type::ERROR);
  }

  if (event_flag & EPOLLRDHUP) {
    types->emplace_back(Event::Type::CLOSE);
  }
}

void EventDispatcherEpoll::OnAttachChannel(int32_t descriptor) {
  epoll_event event;
  event.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
  event.data.fd = descriptor;

  int32_t res = epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, descriptor, &event);
  if (0 > res) {
    LOG(WARNING) << "[EventDispatcherEpoll::OnAttachChannel]"
                 << " fail attach to epoll. " << res;
  }
}

void EventDispatcherEpoll::OnDetachChannel(int32_t descriptor) {
  epoll_ctl(epoll_descriptor_, EPOLL_CTL_DEL, descriptor, nullptr);
}

void EventDispatcherEpoll::OnUpdatedChannel(int32_t descriptor) {
}

}  // namespace base
}  // namespace nlink
