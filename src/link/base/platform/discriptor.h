/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_PLATFORM_LINUX_DISCRIPTOR_H_
#define LINK_BASE_PLATFORM_LINUX_DISCRIPTOR_H_

#include <stdint.h>

namespace link {
namespace base {

using Discriptor = int32_t;

bool SetNoblocking(Discriptor fd);

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_PLATFORM_LINUX_DISCRIPTOR_H_