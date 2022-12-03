/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/kqueue/event_dispatcher_kqueue.h"

#include <sys/event.h>
#include <sys/time.h>
#include <sys/unistd.h>

#include <vector>
#include <array>
#include <utility>

#include "link/base/logging.h"
#include "link/base/event/event_util.h"

namespace nlink {
namespace base {

using KqueueEvent = kevent;
constexpr const int32_t kDefaultEvnetSize = 1024;
const int32_t kDefaultTimeOut = 100;

EventDispatcherKqueue* EventDispatcherKqueue::CreateEventDispatcher() {
  Descriptor kqueue_fd = kqueue();
  if (kqueue_fd < 0) {
    close(kqueue_fd);
    return nullptr;
  }

  return new EventDispatcherKqueue(
    kqueue_fd, kDefaultEvnetSize, kDefaultTimeOut);
}

EventDispatcherKqueue::EventDispatcherKqueue(
  int32_t kqueue_descriptor,
  int32_t event_size,
  int32_t timeout)
  : running_(false),
    event_size_(event_size),
    timeout_(timeout),
    kqueue_descriptor_(kqueue_descriptor),
    event_channel_controller_(nullptr) {}

EventDispatcherKqueue::~EventDispatcherKqueue() {
  close(kqueue_descriptor_);
}

void EventDispatcherKqueue::RegistEventChannelContoller(
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

void EventDispatcherKqueue::Dispatch() {
  while (running_.load()) {
    DispatchOnce();
  }
}

void EventDispatcherKqueue::DispatchOnce() {
  std::array<KqueueEvent, kDefaultEvnetSize> kqeue_events;
  int32_t event_count =
    epoll_wait(epoll_descriptor_, &epoll_events[0], event_size_, timeout_);

  new_events = kevent(
    kqueue_descriptor_,
    &kqeue_events[0], kqeue_events.size(),
    event_list, 8, NULL);

  if (event_count <= 0) {
    return;
  }

  for (int i = 0 ; i < event_count ; ++i) {
    // EventChannel* channel =
      // reinterpret_cast<EventChannel*>(epoll_events[i].data.ptr);
    int32_t fd = epoll_events[i].data.fd;
    uint32_t event_flag = epoll_events[i].fflags;

    LOG(WARNING) << "[EventDispatcherKqueue::dispatch] fd : " << fd;

    std::vector<Event::Type> types;
    HandleEventType(event_flag, &types);

    Event event(fd, types);
    event_channel_controller_->DispatchEvent(event);

    // if (nullptr != channel) {
      // LOG(WARNING) << "[EventDispatcherKqueue::dispatch] channel : " << channel << " / " << fd;
    //   channel->HandleEvent(event);
    // }
  }
}

void EventDispatcherKqueue::HandleEventType(
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

void EventDispatcherKqueue::Stop() {
  running_.store(true);
}

void EventDispatcherKqueue::OnAttachChannel(
  int32_t descriptor, EventChannel*) {
  LOG(WARNING) << "[EventDispatcherKqueue::OnAttachChannel] fd : " << descriptor;

  epoll_event event;
  // event.data.ptr = channel;
  event.events = EPOLLIN | (EPOLLERR | EPOLLRDHUP) | (EPOLLET);
  event.data.fd = descriptor;

  int32_t res = epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, descriptor, &event);
  if (0 > res) {
    LOG(WARNING) << "[EventDispatcherKqueue::OnAttachChannel]"
                 << " fail attach to epoll. " << res;
  }
}

void EventDispatcherKqueue::OnDetachChannel(
  int32_t descriptor, EventChannel*) {
  epoll_ctl(epoll_descriptor_, EPOLL_CTL_DEL, descriptor, nullptr);
}

void EventDispatcherKqueue::OnUpdatedChannel(
  int32_t descriptor, EventChannel*) {
  epoll_event event;
  // event.data.ptr = channel;
  event.events = EPOLLIN | (EPOLLERR | EPOLLRDHUP) | (EPOLLET);
  event.data.fd = descriptor;

  int32_t res = epoll_ctl(epoll_descriptor_, EPOLL_CTL_MOD, descriptor, &event);
  if (0 > res) {
    LOG(WARNING) << "[EventDispatcherKqueue::OnUpdatedChannel]"
                 << " fail attach to epoll. " << res;
  }
}

}  // namespace base
}  // namespace nlink
