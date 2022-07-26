/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/ipc/socket/socket_component.h"

namespace nlink {
namespace component {

SocketComponent::SocketComponent(base::EventChannelController* channel_controller)
  : LinkComponent(channel_controller) {
}

SocketComponent::~SocketComponent() {
}

}  // namespace component
}  // namespace nlink
