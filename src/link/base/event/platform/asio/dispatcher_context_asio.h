/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_ASIO_H_
#define LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_ASIO_H_

#include <map>

#include "link/base/event/event_dispatcher.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/executor_work_guard.hpp"

namespace nlink {
namespace base {

class AsioDispatcherConext : public DispatcherConext {
 public:
  explicit AsioDispatcherConext();
  virtual ~AsioDispatcherConext();

  void* context() const override;

  bool Regist(void* target) override;
  bool Unregist(void* target) override;

 private:
  asio::io_context context_;
  asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_EVENT_PLATFORM_EPOLL_DISPATCHER_CONTEXT_ASIO_H_
