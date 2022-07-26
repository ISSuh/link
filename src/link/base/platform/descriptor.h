/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_PLATFORM_DESCRIPTOR_H_ 
#define LINK_BASE_PLATFORM_DESCRIPTOR_H_

#include <cstdint>

namespace nlink {
namespace base {

using Descriptor = int32_t;

bool SetNoblocking(Descriptor fd);

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_PLATFORM_DESCRIPTOR_H_
