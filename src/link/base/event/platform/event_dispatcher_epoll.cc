/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/event_dispatcher_epoll.h"

#include <sys/epoll.h>
namespace {
#include <sys/unistd.h>
}

#include <vector>
#include <cstring>

#include "link/base/logging.h"

#include <bitset>

namespace link {
namespace base {

using EpollEvent = epoll_event;

EventDispatcherEpoll* EventDispatcherEpoll::CreateEventDispatcher(
  int32_t evnet_size, int32_t timeout) {
  Discriptor epoll_fd = epoll_create(evnet_size);
  if (epoll_fd < 0) {
    ::close(epoll_fd);
    return nullptr;
  }
  return new EventDispatcherEpoll(epoll_fd, evnet_size, timeout);
}

EventDispatcherEpoll::EventDispatcherEpoll(
  Discriptor fd, int32_t event_size, int32_t timeout)
  : epoll_fd_(fd), event_size_(event_size), timeout_(timeout) {
}

EventDispatcherEpoll::~EventDispatcherEpoll() {
  ::close(epoll_fd_);
}

void EventDispatcherEpoll::Dispatch() {
  std::vector<EpollEvent> epoll_events(event_size_);
  int32_t count =
    epoll_wait(epoll_fd_, &epoll_events[0], event_size_, timeout_);

  if (count < 0) {
    LOG(INFO) << __func__ << " - Error";
    return;
  } else if (count == 0) {
    LOG(INFO) << __func__ << " - timeout";
    return;
  }

  LOG(INFO) << __func__ << " - " << count;

  epoll_events.resize(count);
  for (const auto& epoll_event : epoll_events) {
    Discriptor fd = epoll_event.data.fd;
    uint32_t event_flag = epoll_event.events;

    // TODO(issuh) : need test read & send on epoll
    Event::Type type;
    if (event_flag | EPOLLIN) {
      type = Event::Type::READ;
    } else if (event_flag | EPOLLOUT) {
      type = Event::Type::WRITE;
    } else if (event_flag | EPOLLERR) {
      type = Event::Type::ERROR;
    } else if (event_flag | EPOLLRDHUP) {
      type = Event::Type::CLOSE;
    } else {
      type = Event::Type::NONE;
    }

    Event event(fd, type);
    DispatchEvent(event);
  }
}

bool EventDispatcherEpoll::AttachChannel(EventChannel* channel) {
  Discriptor fd = channel->ChannelDiscriptor();
  if (channel_map_.find(fd) != channel_map_.end()) {
    LOG(ERROR) <<  __func__ << " - decriptor already exist.  " << fd;
    return false;
  }

  EpollEvent event;
  event.events = EPOLLIN | EPOLLOUT | 1;
  event.data.fd = fd;

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
    LOG(ERROR) <<  __func__
                << " - fail : " << std::strerror(errno);
    return false;
  }

  channel_map_.insert({fd, channel});
  return true;
}

void EventDispatcherEpoll::DetatchCahnnel(Discriptor fd) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
  channel_map_.erase(fd);
}

void EventDispatcherEpoll::DispatchEvent(const Event& event) {
  Discriptor fd = event.discriptor();
  channel_map_[fd]->HandleEvent(event);
}

}  // namespace base
}  // namespace link
