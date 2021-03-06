/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/socket_component.h"

namespace nlink {
namespace component {

SocketComponent::SocketComponent(base::EventChannelObserver* channel_subject)
  : LinkComponent(channel_subject) {
}

SocketComponent::~SocketComponent() {
}

}  // namespace component
}  // namespace nlink
