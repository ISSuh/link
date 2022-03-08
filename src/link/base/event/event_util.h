/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_EVENT_EVENT_UTIL_H_
#define LINK_BASE_EVENT_EVENT_UTIL_H_

#include <string>

#include "link/base/event/event.h"

namespace link {
namespace base {

std::string EventTypeToString(const Event& event);

}  // namespace base
}  // namespace link

#endif  // LINK_BASE_EVENT_EVENT_UTIL_H_
