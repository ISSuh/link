/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVNET_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
#define LINK_BASE_EVNET_PLATFORM_EVENT_DISPATCHER_EPOLL_H_

#include <sys/epoll.h>
#include <sys/unistd.h>

#include <vector>
#include <cerrno>

#include "link/base/macro.h"
#include "link/base/platform/discriptor.h"
#include "link/base/evnet/event_dispatcher.h"
#include "link/base/logging.h"

namespace link {
namespace base {

class EventDispatcherEpoll : public EventDispatcher {
 public:
  using EpollEvent = epoll_event;

  static EventDispatcherEpoll* CreateEventDispatcher(int32_t evnet_size) {
    Discriptor epoll_fd = epoll_create(evnet_size);
    if (epoll_fd < 0) {
      close(epoll_fd);
      return nullptr;
    }
    return new EventDispatcherEpoll(epoll_fd, evnet_size, 5);
  }

  ~EventDispatcherEpoll() {
    close(epoll_fd_);
  }

  void Dispatch() override {
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

 private:
  EventDispatcherEpoll(Discriptor fd, int32_t event_size, int32_t timeout)
    : epoll_fd_(fd), event_size_(event_size), timeout_(timeout) {}

  bool AttachChannel(EventChannel* channel) override {
    Discriptor fd = channel->ChannelDiscriptor();
    EpollEvent event;
    event.events = EPOLLIN | EPOLLOUT | 1;
    event.data.fd = fd;

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
      LOG(ERROR) <<  __func__
                 << " - fail : " << std::strerror(errno);
      return false;
    }
    return true;

    channel_map_[fd] = channel;
  }

  void DetatchCahnnel(Discriptor fd) override {
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
    channel_map_.erase(fd);
  }

  void DispatchEvent(const Event& event) override {
    Discriptor fd = event.fd();
    channel_map_[fd]->HandleEvent(event);
  }

  Discriptor epoll_fd_;
  uint32_t event_size_;
  int32_t timeout_;

  DISAALOW_COPY_AND_ASSIGN(EventDispatcherEpoll);
};

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVNET_PLATFORM_EVENT_DISPATCHER_EPOLL_H_
