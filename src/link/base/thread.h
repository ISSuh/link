/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_THREAD_H_
#define LINK_BASE_THREAD_H_

#include <cstdint>

namespace nlink {
namespace base {

uint64_t CurrentThreadId();

uint64_t HashedCurrentThreadId();

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_THREAD_H_
