/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_

#include "link/base/macro.h"
#include "link/base/event/event_observer.h"

namespace link {
namespace component {

class SocketComponent : public base::EventObserver {
 public:
 private:
  SocketComponent();
  ~SocketComponent();

  DISAALOW_COPY_AND_ASSIGN(SocketComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_
