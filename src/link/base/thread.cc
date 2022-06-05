/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/thread.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <thread>

namespace nlink {
namespace base {

#define SYS_gettid __NR_gettid

uint64_t CurrentThreadId() {
  size_t tid = static_cast<size_t>(syscall(SYS_gettid));
  return tid;
}

uint64_t HashedCurrentThreadId() {
  return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

}  // namespace base
}  // namespace nlink

