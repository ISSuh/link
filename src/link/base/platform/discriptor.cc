/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/platform/linux/discriptor.h"

#include <fcntl.h>

namespace nlink {
namespace base {

bool SetNoblocking(Discriptor fd) {
  const int32_t flags = fcntl(fd, F_GETFL);

  if (flags == -1) {
    return false;
  }

  if (flags & O_NONBLOCK) {
    return true;
  }

  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    return false;
  }
  return true;
}

}  // namespace base
}  // namespace nlink
