/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_EPOLL_H_
#define LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_EPOLL_H_

#include <map>

#include "link/base/event/event_dispatcher.h"

namespace nlink {
namespace base {

class EpollDispatcherConext : public DispatcherConext {
 public:
  explicit EpollDispatcherConext(int32_t discriptor);
  virtual ~EpollDispatcherConext();

  void* context() const override;

  bool Regist(int32_t handle, EventChannel* channel) override;
  bool Unregist(int32_t handle) override;

  void Dispatch();

 private:
  int32_t discriptor_;
  std::map<int32_t, EventChannel*> channels;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_EPOLL_H_
