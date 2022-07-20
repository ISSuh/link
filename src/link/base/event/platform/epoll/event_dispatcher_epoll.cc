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
  while (1) {
    DispatchOnce();
  }
}

void EventDispatcherEpoll::DispatchOnce() {
  int32_t context = *(int32_t*)context_->context();

  std::array<EpollEvent, kDefaultEvnetSize> epoll_events;
  int32_t event_count =
    epoll_wait(context, &epoll_events[0], event_size_, timeout_);
  if (event_count < 0) {
    LOG(INFO) << __func__ << " - Error";
    return;
  } else if (event_count == 0) {
    LOG(INFO) << __func__ << " - timeout";
    return;
  }

  LOG(INFO) << __func__ << " - event_count : " << event_count;


  for (int i = 0 ; i < event_count ; ++i) {
    int32_t fd = epoll_events[i].data.fd;
    uint32_t event_flag = epoll_events[i].events;

    std::vector<Event::Type> types;

    HandleEvent(event_flag, &types);

    Event event(fd, types);
    DispatchEvent(event);
  }
}

DispatcherConext* EventDispatcherEpoll::GetDispatcherConext() {
  return context_.get();
}

Event::Type EventDispatcherEpoll::HandleEvent(
  uint32_t event_flag, std::vector<Event::Type>* types) {
  if (event_flag & EPOLLIN) {
    types->emplace_back(Event::Type::READ);
  }

  if (event_flag & EPOLLOUT) {
    types->emplace_back(Event::Type::WRITE);
  }
  
  if (types->empty()) {
    types->emplace_back(Event::Type::ACCEPT);
  }

  if (event_flag & EPOLLERR) {
    types->emplace_back(Event::Type::ERROR);
  }

  if (event_flag & EPOLLRDHUP) {
    types->emplace_back(Event::Type::CLOSE);
  }
}

void EventDispatcherEpoll::AttachChannels(EventChannel* channel) {
  channel->OpenChannel(context_.get());
}

void EventDispatcherEpoll::DetatchCahnnel(EventChannel* channel) {
}

void EventDispatcherEpoll::DispatchEvent(const Event& event) {
  context_->Dispatch(event);
}

}  // namespace base
}  // namespace nlink
