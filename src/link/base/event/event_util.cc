/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/event/event_util.h"

namespace nlink {
namespace base {

std::string EventTypeToString(const Event& event) {
  switch (event.type()) {
    case Event::Type::ERROR:
      return "Event::Type::ERROR";
    case Event::Type::NONE:
      return "Event::Type::NONE";
    case Event::Type::ACCEPT:
      return "Event::Type::ACCEPT";
    case Event::Type::CONNECT:
      return "Event::Type::CONNECT";
    case Event::Type::READ:
      return "Event::Type::READ";
    case Event::Type::WRITE:
      return "Event::Type::WRITE";
    case Event::Type::TIMEOUT:
      return "Event::Type::TIMEOUT";
    case Event::Type::CLOSE:
      return "Event::Type::CLOSE";
    default:
      return "Invalid event type";
  }
}

}  // namespace base
}  // namespace nlink
