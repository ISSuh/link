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

AsioDispatcherConext::AsioDispatcherConext()
  : context_(1),
    work_guard_(context_.get_executor()) {
}

AsioDispatcherConext::~AsioDispatcherConext() {
}

void* AsioDispatcherConext::context() const {
  return (void*)(&Descriptor_);
}

bool AsioDispatcherConext::Regist(void* target) {

}

bool AsioDispatcherConext::Unregist(void* target) {

}

}  // namespace base
}  // namespace nlink
