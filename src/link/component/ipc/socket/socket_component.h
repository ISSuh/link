/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_

#include <string>

#include "link/base/macro.h"
#include "link/base/event/event_observer.h"
#include "link/component/base/component.h"

namespace nlink {
namespace component {

class SocketComponent : public base::EventObserver,
                        public LinkComponent {
 protected:
  SocketComponent(
    const std::string& name, base::EventChannelController* event_controller)
    : LinkComponent(name, event_controller) {}

  virtual ~SocketComponent() = default;

  DISAALOW_COPY_AND_ASSIGN(SocketComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_
