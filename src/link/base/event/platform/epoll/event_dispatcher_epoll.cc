/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/platform/epoll/event_dispatcher_epoll.h"

#include <sys/epoll.h>
#include <sys/unistd.h>

#include <vector>
#include <utility>

#include "link/base/logging.h"


namespace nlink {
namespace base {

using EpollEvent = epoll_event;
const int32_t kDefaultEvnetSize = 1024;
const int32_t kDefaultTimeOut = 60;

EventDispatcherEpoll* EventDispatcherEpoll::CreateEventDispatcher() {
  Discriptor epoll_fd = epoll_create(kDefaultEvnetSize);
  if (epoll_fd < 0) {
    close(epoll_fd);
    return nullptr;
  }

  std::unique_ptr<EpollDispatcherConext> context =
    std::make_unique<EpollDispatcherConext>(epoll_fd);

  return new EventDispatcherEpoll(
    std::move(context), kDefaultEvnetSize, kDefaultTimeOut);
}

EventDispatcherEpoll::EventDispatcherEpoll(
  std::unique_ptr<EpollDispatcherConext> context,
  int32_t event_size,
  int32_t timeout)
  : context_(std::move(context)), event_size_(event_size), timeout_(timeout) {
}

EventDispatcherEpoll::~EventDispatcherEpoll() {
}

void EventDispatcherEpoll::Dispatch() {
}

void EventDispatcherEpoll::DispatchOnce() {
  int32_t context = *(int32_t*)context_->context();
  std::vector<EpollEvent> epoll_events(event_size_);
  int32_t count =
    epoll_wait(context, &epoll_events[0], event_size_, timeout_);

  if (count < 0) {
    LOG(INFO) << __func__ << " - Error";
    return;
  } else if (count == 0) {
    LOG(INFO) << __func__ << " - timeout";
    return;
  }

  epoll_events.resize(count);
  for (const auto& epoll_event : epoll_events) {
    int32_t fd = epoll_event.data.fd;
    uint32_t event_flag = epoll_event.events;

    // Event::Type type;
    // switch (channel_map_[fd]->ObserverType()) {
    //   case EventObserver::Type::SERVER: {
    //     type = HandlingServerEvent();
    //     break;
    //   }
    //   case EventObserver::Type::CLIENT: {
    //     type = HandlingClientEvent(event_flag);
    //     break;
    //   }
    // }

    // Event event(fd, type);
    // DispatchEvent(event);
  }
}

DispatcherConext* EventDispatcherEpoll::GetDispatcherConext() {
  return context_.get();
}

Event::Type EventDispatcherEpoll::HandlingServerEvent() {
  return Event::Type::ACCEPT;
}

Event::Type EventDispatcherEpoll::HandlingClientEvent(uint32_t event_flag) {
  Event::Type type;
  if (event_flag & EPOLLIN) {
    LOG(INFO) << __func__ << " - Event::Type::READ;";
    type = Event::Type::READ;
  }

  if (event_flag & EPOLLOUT) {
    LOG(INFO) << __func__ << " - Event::Type::WRITE";
    type = Event::Type::WRITE;
  }

  if (event_flag & EPOLLERR) {
    LOG(INFO) << __func__ << " - Event::Type::ERROR";
    type = Event::Type::ERROR;
  }

  if (event_flag & EPOLLRDHUP) {
    LOG(INFO) << __func__ << " - Event::Type::CLOSE";
    type = Event::Type::CLOSE;
  }
  return type;
}

void EventDispatcherEpoll::AttachChannels(EventChannel* channel) {
  channel->OpenChannel(context_.get());
}

void EventDispatcherEpoll::DetatchCahnnel(EventChannel* channel) {
}

void EventDispatcherEpoll::DispatchEvent(const Event& event) {
}

}  // namespace base
}  // namespace nlink
