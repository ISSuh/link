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

EventDispatcherEpoll* EventDispatcherEpoll::CreateEventDispatcher() {
  Descriptor epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    close(epoll_fd);
    return nullptr;
  }

  return new EventDispatcherEpoll(epoll_fd, kDefaultEvnetSize, kDefaultTimeOut);
}

EventDispatcherEpoll::EventDispatcherEpoll(
  int32_t epoll_descriptor,
  int32_t event_size,
  int32_t timeout)
  : running_(false),
    epoll_descriptor_(epoll_descriptor),
    event_size_(event_size),
    timeout_(timeout),
    event_channel_controller_(nullptr) {}

EventDispatcherEpoll::~EventDispatcherEpoll() {
  close(epoll_descriptor_);
}

void EventDispatcherEpoll::RegistEventChannelContoller(
  std::shared_ptr<ChannelController> channel_controller)  {
  channel_controller->RegistEventChannelDelegateCallback(
        [this](int32_t descriptor, EventChannel* channel) {
          this->OnAttachChannel(descriptor, channel);
        },
        [this](int32_t descriptor, EventChannel* channel) {
          this->OnDetachChannel(descriptor, channel);
        },
        [this](int32_t descriptor, EventChannel* channel) {
          this->OnUpdatedChannel(descriptor, channel);
        });

  event_channel_controller_ = channel_controller;

  running_.store(true);
}

void EventDispatcherEpoll::Dispatch() {
  while (running_.load()) {
    DispatchOnce();
  }
}

void EventDispatcherEpoll::DispatchOnce() {
  std::array<EpollEvent, kDefaultEvnetSize> epoll_events;
  int32_t event_count =
    epoll_wait(epoll_descriptor_, &epoll_events[0], event_size_, timeout_);
  if (event_count <= 0) {
    return;
  }

  for (int i = 0 ; i < event_count ; ++i) {
    // EventChannel* channel =
      // reinterpret_cast<EventChannel*>(epoll_events[i].data.ptr);
    int32_t fd = epoll_events[i].data.fd;
    uint32_t event_flag = epoll_events[i].events;

    LOG(WARNING) << "[EventDispatcherEpoll::dispatch] fd : " << fd;

    std::vector<Event::Type> types;
    HandleEventType(event_flag, &types);

    Event event(fd, types);
    event_channel_controller_->DispatchEvent(event);

    // if (nullptr != channel) {
      // LOG(WARNING) << "[EventDispatcherEpoll::dispatch] channel : " << channel << " / " << fd;
    //   channel->HandleEvent(event);
    // }
  }
}

Event::Type EventDispatcherEpoll::HandleEventType(
  uint32_t event_flag, std::vector<Event::Type>* types) {
  if (event_flag & EPOLLIN) {
    LOG(INFO) << __func__ << " - EPOLLIN";
    types->emplace_back(Event::Type::READ);
  }

  if (event_flag & EPOLLOUT) {
    LOG(INFO) << __func__ << " - EPOLLOUT";
    types->emplace_back(Event::Type::WRITE);
  }

  if (event_flag & EPOLLERR) {
    LOG(INFO) << __func__ << " - EPOLLERR";
    types->emplace_back(Event::Type::ERROR);
  }

  if (event_flag & EPOLLRDHUP) {
    LOG(INFO) << __func__ << " - EPOLLRDHUP";
    types->emplace_back(Event::Type::CLOSE);
  }

  if (event_flag & EPOLLHUP) {
    LOG(INFO) << __func__ << " - EPOLLHUP";
    types->emplace_back(Event::Type::CLOSE);
  }
}

void EventDispatcherEpoll::Stop() {
  running_.store(true);
}

void EventDispatcherEpoll::OnAttachChannel(
  int32_t descriptor, EventChannel* channel) {
    LOG(WARNING) << "[EventDispatcherEpoll::OnAttachChannel] fd : " << descriptor;

  epoll_event event;
  // event.data.ptr = channel;
  event.events = EPOLLIN | (EPOLLERR | EPOLLRDHUP) | (EPOLLET);
  event.data.fd = descriptor;

  int32_t res = epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, descriptor, &event);
  if (0 > res) {
    LOG(WARNING) << "[EventDispatcherEpoll::OnAttachChannel]"
                 << " fail attach to epoll. " << res;
  }
}

void EventDispatcherEpoll::OnDetachChannel(
  int32_t descriptor, EventChannel* channel) {
  epoll_ctl(epoll_descriptor_, EPOLL_CTL_DEL, descriptor, nullptr);
}

void EventDispatcherEpoll::OnUpdatedChannel(
  int32_t descriptor, EventChannel* channel) {
  epoll_event event;
  // event.data.ptr = channel;
  event.events = EPOLLIN | (EPOLLERR | EPOLLRDHUP) | (EPOLLET);
  event.data.fd = descriptor;

  int32_t res = epoll_ctl(epoll_descriptor_, EPOLL_CTL_MOD, descriptor, &event);
  if (0 > res) {
    LOG(WARNING) << "[EventDispatcherEpoll::OnUpdatedChannel]"
                 << " fail attach to epoll. " << res;
  }
}

}  // namespace base
}  // namespace nlink
